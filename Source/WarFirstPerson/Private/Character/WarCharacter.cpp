// Personal Copyright
#include "Character/WarCharacter.h"
#include "Character/WarCharacterMovementComponent.h"
#include "Character/WarHealthComponent.h"
#include "Character/WarPawnComponent.h"
#include "Character/WarPawnData.h"
#include "Camera/WarCameraComponent.h"
#include "Camera/WarCameraMode.h"
#include "Camera/WarFirstPersonCameraMode.h"
#include "Camera/WarThirdPersonCameraMode.h"
#include "Camera/WarAimDownSightCameraMode.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "Input/WarEnhancedInputComponent.h"
#include "Input/WarInputConfig.h"
#include "Input/WarPlayerMappableInputConfigPair.h"
#include "InteractiveActor/WarInteractiveActor.h"
#include "Inventory/WarInventoryComponent.h"
#include "Inventory/WarInventoryItemDefinition.h"
#include "PlayerMappableInputConfig.h"
#include "Player/WarLocalPlayer.h"
#include "Player/WarPlayerController.h"
#include "Player/WarPlayerState.h"
#include "WarFirstPerson/WarGameplayTags.h"
#include "WarFirstPerson/WarLogChannels.h"
#include "Weapon/WarWeapon.h"

// Sets default values
AWarCharacter::AWarCharacter(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer.SetDefaultSubobjectClass<UWarCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsArmed = false;
	bIsCrouched = false;
	bIsAimingDownSight = false;
	bIsMelee = false;
	UWarCharacterMovementComponent* WarCharacterMovementComponent = CastChecked<UWarCharacterMovementComponent>(GetCharacterMovement());
	WarCharacterMovementComponent->bAllowPhysicsRotationDuringAnimRootMotion = false;
	WarCharacterMovementComponent->bCanWalkOffLedgesWhenCrouching = true;
	WarCharacterMovementComponent->bOrientRotationToMovement = false;
	WarCharacterMovementComponent->bUseControllerDesiredRotation = false;
	WarCharacterMovementComponent->BrakingDecelerationWalking = 1400.0f;
	WarCharacterMovementComponent->BrakingFriction = 6.0f;
	WarCharacterMovementComponent->BrakingFrictionFactor = 1.0f;
	WarCharacterMovementComponent->GravityScale = 1.0f;
	WarCharacterMovementComponent->MaxAcceleration = 2400.0f;
	WarCharacterMovementComponent->NavAgentProps.bCanCrouch = 1;
	WarCharacterMovementComponent->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	WarCharacterMovementComponent->SetCrouchedHalfHeight(65.0f);

	WarHealthComponent = CreateDefaultSubobject<UWarHealthComponent>(TEXT("WarHealthComponent"));

	WarInventoryComponent = CreateDefaultSubobject<UWarInventoryComponent>(TEXT("WarInventoryComponent"));

	WarPawnComponent = CreateDefaultSubobject<UWarPawnComponent>(TEXT("WarPawnComponent"));

	WarCameraComponent = CreateDefaultSubobject<UWarCameraComponent>(TEXT("WarCameraComponent"));
	WarCameraComponent->SetupAttachment(GetMesh(), FName("head"));
	WarCameraComponent->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AWarCharacter::BeginPlay()
{
	Super::BeginPlay();
	UWarCameraMode* ThirdPersonWarCameraMode = NewObject<UWarCameraMode>(WarCameraComponent, WarCameraComponent->GetDefaultThirdPersonCameraMode(), NAME_None, RF_NoFlags);
	check(ThirdPersonWarCameraMode);
	WarCameraComponent->SetCurrentWarCameraMode(ThirdPersonWarCameraMode);
}

bool AWarCharacter::AddOverlappedActor(AActor* Actor)
{
	if (AWarInteractiveActor* InteractiveActor = Cast<AWarInteractiveActor>(Actor))
	{
		if (OverlappedActors.Add(InteractiveActor) != INDEX_NONE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool AWarCharacter::RemoveOverlappedActor(AActor* Actor)
{
	if (AWarInteractiveActor* InteractiveActor = Cast<AWarInteractiveActor>(Actor))
	{
		int32 WarInteractiveActorIndex = OverlappedActors.Find(InteractiveActor);
		if (WarInteractiveActorIndex != INDEX_NONE)
		{
			OverlappedActors.RemoveAt(WarInteractiveActorIndex);
			return true;
		}
	}
	return false;
}

bool AWarCharacter::IsArmed()
{
	return bIsArmed;
}

void AWarCharacter::SetIsArmed(bool Armed)
{
	this->bIsArmed = Armed;
}

void AWarCharacter::Move(const FInputActionValue& InputActionValue)
{

	if (!Controller)
	{
		return;
	}
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

	if (Value.X != 0.0f) // 二维向量中的X值负代表向左，正代表向右
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, Value.X);
	}
	if (Value.Y != 0.0f) // 二维向量中的Y轴负代表向后，正代表向前
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, Value.Y);
	}
}

void AWarCharacter::Turn(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		AddControllerYawInput(Value.X);
	}
	if (Value.Y != 0.0f)
	{
		AddControllerPitchInput(Value.Y);
	}
}

void AWarCharacter::Interact()
{
	if (HighestPriorityActor)
	{
		HighestPriorityActor->Interact(Cast<APawn>(this));
	}
}

void AWarCharacter::Fire()
{
	if (!bIsArmed)
	{
		return ;
	}
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* AttachedActor : AttachedActors)
	{
		if (AWarWeapon* CurrentEquippedWarWeapon = Cast<AWarWeapon>(AttachedActor))
		{
			CurrentEquippedWarWeapon->Fire();
		}
	}
}

void AWarCharacter::AimDownSight()
{
	if (bIsArmed && !bIsAimingDownSight)
	{
		bIsAimingDownSight = true;
		UWarCameraMode* AimDownSightWarCameraMode = NewObject<UWarCameraMode>(WarCameraComponent, WarCameraComponent->GetDefaultAimDownSightCameraMode(), NAME_None, RF_NoFlags);
		if (AimDownSightWarCameraMode)
		{
			WarCameraComponent->SetCurrentWarCameraMode(AimDownSightWarCameraMode);
		}
	}
}

void AWarCharacter::EndAimDownSight()
{
	if (bIsArmed && bIsAimingDownSight)
	{
		bIsAimingDownSight = false;
		UWarCameraMode* ThirdPersonWarCameraMode = NewObject<UWarCameraMode>(WarCameraComponent, WarCameraComponent->GetDefaultThirdPersonCameraMode(), NAME_None, RF_NoFlags);
		check(ThirdPersonWarCameraMode);
		WarCameraComponent->SetCurrentWarCameraMode(ThirdPersonWarCameraMode);
	}
}

void AWarCharacter::CycleActiveWeaponForward()
{
	WarInventoryComponent->CycleActiveWeaponForward();
}

void AWarCharacter::CycleActiveWeaponBackward()
{
	WarInventoryComponent->CycleActiveWeaponBackward();
}

#if UE_BUILD_DEVELOPMENT
void AWarCharacter::ToggleDebugWeidget()
{

}
#endif

void AWarCharacter::Jump()
{
	if (bIsCrouched)
	{
		Super::UnCrouch();
	}
	Super::Jump();
}

void AWarCharacter::ToggleCrouch()
{
	if (bIsCrouched)
	{
		Super::UnCrouch();
	}
	else
	{
		Super::Crouch();
	}
}

// Called every frame
void AWarCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FHitResult HitResult;
	FCollisionQueryParams QueryParameters;
	QueryParameters.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, WarCameraComponent->GetComponentLocation(), WarCameraComponent->GetForwardVector(), ECollisionChannel::ECC_WorldStatic, QueryParameters);
	if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
	{
		AWarInteractiveActor* WarInteractiveActor = Cast<AWarInteractiveActor>(HitResult.GetActor());
		if (WarInteractiveActor)
		{
			if (OverlappedActors.Find(WarInteractiveActor))
			{
				this->HighestPriorityActor = WarInteractiveActor;
			}
		}
		else if(!OverlappedActors.IsEmpty())
		{
			this->HighestPriorityActor = OverlappedActors.Last();
		}
	}
}

AWarPlayerController* AWarCharacter::GetWarPlayerController() const
{
	return CastChecked<AWarPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

AWarPlayerState* AWarCharacter::GetWarPlayerState() const
{
	return CastChecked<AWarPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

// Called to bind functionality to input
void AWarCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent)

	const APlayerController* PlayerController = GetController<APlayerController>();
	check(PlayerController)
	const UWarLocalPlayer* WarLocalPlayer = Cast<UWarLocalPlayer>(PlayerController->GetLocalPlayer());
	check(WarLocalPlayer)
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = WarLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(EnhancedInputLocalPlayerSubsystem)
	EnhancedInputLocalPlayerSubsystem->ClearAllMappings();

	check(WarPawnComponent)
	const UWarPawnData* WarPawnData = WarPawnComponent->GetPawnData<UWarPawnData>();
	check(WarPawnData)
	const UWarInputConfig* WarInputConfig = WarPawnData->WarInputConfig;
	check(WarInputConfig)
	const FWarGameplayTags& WarGameplayTags = FWarGameplayTags::Get();
	for (const FWarPlayerMappableInputConfigPair& PlayerMappableConfigPair : DefaultInputConfigs)
	{
		if (PlayerMappableConfigPair.bShouldActivateAutomatically && PlayerMappableConfigPair.CanBeActivated())
		{
			FModifyContextOptions ModifyContextOptions = {};
			ModifyContextOptions.bIgnoreAllPressedKeysUntilRelease = false;
			EnhancedInputLocalPlayerSubsystem->AddPlayerMappableConfig(PlayerMappableConfigPair.PlayerMappableInputConfig.LoadSynchronous(), ModifyContextOptions);
		}
	}

	UWarEnhancedInputComponent* WarEnhancedInputComponent = CastChecked<UWarEnhancedInputComponent>(PlayerInputComponent);
	WarEnhancedInputComponent->AddInputMappings(WarInputConfig, EnhancedInputLocalPlayerSubsystem);
#if UE_BUILD_DEVELOPMENT
	if (const UInputAction* InputAction = WarInputConfig->FindNativeInputActionForTag(WarGameplayTags.InputToggleDebugInformation, false))
	{
		WarEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::ToggleDebugWeidget);
	}
#endif
	if (const UInputAction* InputAction = WarInputConfig->FindNativeInputActionForTag(WarGameplayTags.InputMove, false))
	{
		WarEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	}
	if (const UInputAction* InputAction = WarInputConfig->FindNativeInputActionForTag(WarGameplayTags.InputTurn, false))
	{
		WarEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::Turn);
	}
	if (const UInputAction* InputAction = WarInputConfig->FindNativeInputActionForTag(WarGameplayTags.InputJump, false))
	{
		WarEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::Jump);
	}
	if (const UInputAction* InputAction = WarInputConfig->FindNativeInputActionForTag(WarGameplayTags.InputCrouch, false))
	{
		WarEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::ToggleCrouch);
	}
	if (const UInputAction* InputAction = WarInputConfig->FindNativeInputActionForTag(WarGameplayTags.InputInteract, false))
	{
		WarEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::Interact);
	}
	if (const UInputAction* InputAction = WarInputConfig->FindNativeInputActionForTag(WarGameplayTags.InputFire, false))
	{
		WarEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::Fire);
	}
	if (const UInputAction* InputAction = WarInputConfig->FindNativeInputActionForTag(WarGameplayTags.InputAimDownSight, false))
	{
		WarEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::AimDownSight);
	}
	if (const UInputAction* InputAction = WarInputConfig->FindNativeInputActionForTag(WarGameplayTags.InputEndAimDownSight, false))
	{
		WarEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::EndAimDownSight);
	}
	if (const UInputAction* InputAction = WarInputConfig->FindNativeInputActionForTag(WarGameplayTags.InputCycleActiveWeaponForward, false))
	{
		WarEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::CycleActiveWeaponForward);
	}
	if (const UInputAction* InputAction = WarInputConfig->FindNativeInputActionForTag(WarGameplayTags.InputCycleActiveWeaponBackward, false))
	{
		WarEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::CycleActiveWeaponBackward);
	}
}
