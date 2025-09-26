// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReplicationTestCharacter.h"
#include "InventoryComp.h"
#include "ReplicationTestProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlantActor.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// AReplicationTestCharacter

AReplicationTestCharacter::AReplicationTestCharacter()
{
	// Set whether this actor ticks.  Can be turned off to improve performance
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	InventoryComp = CreateDefaultSubobject<UInventoryComp>(TEXT("Inventory Component"));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	bAltInteractKeyHeld = false;
	TimeAltInteractKeyHeld = 0.f;
	bCanMove = true;
}

void AReplicationTestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	GetWorld()->GetTimerManager().SetTimer(CheckForInteractiveTimer, this, &AReplicationTestCharacter::CheckForInteractive, 0.1f, true, 0.1f);
}

void AReplicationTestCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ActorInView && bAltInteractKeyHeld)
	{
		DealWithHeldAltInteract(DeltaSeconds);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AReplicationTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AReplicationTestCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AReplicationTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AReplicationTestCharacter::MoveRight);

	// Interact events
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AReplicationTestCharacter::TryToInteract);

	// Alternative Interact Events
	PlayerInputComponent->BindAction("AltInteract", IE_Pressed, this, &AReplicationTestCharacter::OnAltInteractKeyPressed);
	PlayerInputComponent->BindAction("AltInteract", IE_Released, this, &AReplicationTestCharacter::OnAltInteractKeyReleased);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AReplicationTestCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AReplicationTestCharacter::LookUpAtRate);
}

void AReplicationTestCharacter::OnFire()
{
	if (!HasAuthority())
	{
		OnServer_FireProjectile();
	}
	else
	{
		if (OverlappingPlant)
		{
			OverlappingPlant->ChangeHealth(-30.f);
		}
		else
		{
			// try and fire a projectile
			if (ProjectileClass != nullptr)
			{
				UWorld* const World = GetWorld();
				if (World != nullptr)
				{/*
					if (bUsingMotionControllers)
					{
						const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
						const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
						World->SpawnActor<AReplicationTestProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
					}
					else
					{
						const FRotator SpawnRotation = GetControlRotation();
						// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
						const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

						// spawn the projectile at the muzzle
						AReplicationTestProjectile* NewProjectile = World->SpawnActor<AReplicationTestProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
						NewProjectile->SetOwner(this);
					}
					*/
				}
			}

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			// try and play a firing animation if specified
			if (FireAnimation != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(FireAnimation, 1.f);
				}
			}
		}
	}
}

void AReplicationTestCharacter::MoveForward(float Value)
{
	if (bCanMove)
	{
		if (Value != 0.0f)
		{
			// add movement in that direction
			AddMovementInput(GetActorForwardVector(), Value);
		}
	}
}

void AReplicationTestCharacter::MoveRight(float Value)
{
	if (bCanMove)
	{
		if (Value != 0.0f)
		{
			// add movement in that direction
			AddMovementInput(GetActorRightVector(), Value);
		}
	}
}

void AReplicationTestCharacter::TurnAtRate(float Rate)
{
	if (bCanMove)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AReplicationTestCharacter::LookUpAtRate(float Rate)
{
	if (bCanMove)
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AReplicationTestCharacter::SetPlantActor(APlantActor* PlantBeingOverlapped)
{
	if (!HasAuthority())
	{
		Server_SetPlantActor(PlantBeingOverlapped);
		return;
	}
	
	if (OverlappingPlant != PlantBeingOverlapped)
	{
		OverlappingPlant = PlantBeingOverlapped;
	}
}

void AReplicationTestCharacter::SetViewedActor(AActor* ActorIn)
{
	OnServer_SetViewedActor(ActorIn);
}

void AReplicationTestCharacter::SetCanMove(const bool CanMove)
{
	bCanMove = CanMove;
}

void AReplicationTestCharacter::CheckForInteractive()
{
	/*
	FVector StartLoc = GetActorLocation();
	FVector EndLoc = (GetActorForwardVector() * 500.f) + StartLoc;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	// Perform a line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, QueryParams))
	{
		// Check if what has been hit implements an interface
		if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UInteractInterface::StaticClass()))
		{
			// Check if a new actor has been hit
			if (ActorInView != HitResult.GetActor())
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor hit is new and is %s"), *HitResult.GetActor()->GetName());
				Execute_OnActorViewed(HitResult.GetActor());
				SetViewedActor(HitResult.GetActor());
			}
		}
	}
	// Nothing has been hit.  Check if an actor is referenced, clear it if it is
	else if (ActorInView)
	{
		Execute_OnActorViewLost(ActorInView);
		SetViewedActor(nullptr);
	}
	*/
}

void AReplicationTestCharacter::OnAltInteractKeyPressed()
{
	bAltInteractKeyHeld = true;
}

void AReplicationTestCharacter::OnAltInteractKeyReleased()
{
	bAltInteractKeyHeld = false;
	TimeAltInteractKeyHeld = 0.f;
}

void AReplicationTestCharacter::TryToInteract()
{
	if (InventoryComp)
	{
		InventoryComp->DealWithInteract(this);
	}
	/*
	if (ActorInView && UKismetSystemLibrary::DoesImplementInterface(ActorInView, UInteractInterface::StaticClass()))
	{
		Execute_OnActorInteracted(ActorInView, this);
	}
	*/
}

void AReplicationTestCharacter::DealWithHeldAltInteract(float DeltaTime)
{
	TimeAltInteractKeyHeld += DeltaTime;
	if (TimeAltInteractKeyHeld > 1.0f)
	{
		Execute_OnActorInteracted(ActorInView, this);
		// Force the key to be released
		OnAltInteractKeyReleased();
	}
}

void AReplicationTestCharacter::OnServer_SetViewedActor_Implementation(AActor* ActorBeingViewed)
{
	if (!HasAuthority())
	{
		SetViewedActor(ActorBeingViewed);
		return;
	}

	if (ActorBeingViewed != ActorInView)
	{
		ActorInView = ActorBeingViewed;
	}
}

bool AReplicationTestCharacter::OnServer_SetViewedActor_Validate(AActor* ActorBeingViewed)
{
	return true;
}

void AReplicationTestCharacter::Server_SetPlantActor_Implementation(APlantActor* PlantActor)
{
	SetPlantActor(PlantActor);
}

void AReplicationTestCharacter::OnServer_FireProjectile_Implementation()
{
	OnFire();
}

bool AReplicationTestCharacter::OnServer_FireProjectile_Validate()
{
	return true;
}

void AReplicationTestCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AReplicationTestCharacter, OverlappingPlant);
	DOREPLIFETIME(AReplicationTestCharacter, ActorInView);
}
