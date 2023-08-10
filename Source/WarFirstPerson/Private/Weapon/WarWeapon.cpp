// Personal Copyright
#include "Weapon/WarWeapon.h"
#include "AIController.h"
#include "Camera/WarCameraMode.h"
#include "Camera/WarCameraComponent.h"
#include "Character/WarCharacter.h"
#include "Character/WarHealthComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/HitResult.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NativeGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "Inventory/WarInventoryComponent.h"
#include "Inventory/WarInventoryItemInstance.h"
#include "Team/WarTeamManagerSubsystem.h"
#include "Physics/WarPhysicalMaterial.h"
#include "Player/WarPlayerController.h"
#include "Weapon/WarWeaponAmmunitionComponent.h"
#include "Weapon/WarWeaponPropertiesComponent.h"
#include "Weapon/WarWeaponStateComponent.h"
#include "Weapon/WarWeaponProjectile.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(GameplayTagHitZone, "HitZone")

// Sets default values
AWarWeapon::AWarWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	SetRootComponent(WeaponSkeletalMesh);
	WeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponSkeletalMesh->SetSimulatePhysics(true);
	WeaponSkeletalMesh->SetEnableGravity(true);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Bullet = CreateDefaultSubobject<AWarWeaponProjectile>(TEXT("WarWeaponProjectile"));

	WarWeaponAmmunitionComponent = CreateDefaultSubobject<UWarWeaponAmmunitionComponent>(TEXT("WarWeaponAmmunitionComponent"));

	WarWeaponPropertiesComponent = CreateDefaultSubobject<UWarWeaponPropertiesComponent>(TEXT("WarWeaponPropertiesComponent"));

	WarWeaponStateComponent = CreateDefaultSubobject<UWarWeaponStateComponent>(TEXT("WarWeaponStateComponent"));
	WarWeaponStateComponent->SetCurrentWarWeaponState(EWarWeaponState::NoOwner);
}

// Called every frame
void AWarWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const bool bMinSpread = UpdateSpread(DeltaTime);
	const bool bMinMultipliers = UpdateMultipliers(DeltaTime);
	WarWeaponPropertiesComponent->SetAllowFirstShotAccracy(WarWeaponPropertiesComponent->GetAllowFirstShotAccuracy() && bMinSpread && bMinMultipliers);
}

void AWarWeapon::Interact(APawn* Pawn)
{
	Super::Interact(Pawn);
	PickupWeapon(Pawn);
}

void AWarWeapon::Fire()
{
	if (WarWeaponStateComponent->GetCurrentWarWeaponState() != EWarWeaponState::Ready)
	{
		return;
	}
	bIsFiring = true;
	OnFiring();
	int32 CurrentMagazineCapacity = WarWeaponAmmunitionComponent->GetCurrentMagazineCapacity();
	if (CurrentMagazineCapacity <= 0)
	{
		return;
	}
	WarWeaponStateComponent->SetCurrentWarWeaponState(EWarWeaponState::Firing);
	const AActor* OwnerActor = GetOwner();
	check(OwnerActor);
	const AWarCharacter* OwnerWarCharacter = Cast<AWarCharacter>(OwnerActor);
	check(OwnerWarCharacter);
	const AController* Controller = OwnerWarCharacter->GetController();
	check(Controller);
	const APlayerController* PlayerController = OwnerWarCharacter->GetController<APlayerController>();
	check(PlayerController);
	TArray<FHitResult> HitResults;
	Targeting(HitResults);
	//if (HitResults.Num() > 0)
	//{
	//	const int32 BulletsIdentity = FMath::Rand();
	//	for (const FHitResult& HitResult : HitResults)
	//	{

	//	}
	//}
	AddWeaponSpread();
	WarWeaponAmmunitionComponent->SetCurrentMagazineCapacity(CurrentMagazineCapacity--);
	for (const FHitResult& HitResult : HitResults)
	{
		const AActor* HitActor = HitResult.HitObjectHandle.FetchActor();
		FVector ImpactLocation = FVector::ZeroVector;
		FVector ImpactNormal = FVector::ZeroVector;
		FVector TraceStart = FVector::ZeroVector;
		FVector TraceEnd = FVector::ZeroVector;
		if (HitActor)
		{
			ImpactLocation = HitResult.ImpactPoint;
			ImpactNormal = HitResult.ImpactNormal;
			TraceStart = HitResult.TraceStart;
			TraceEnd = HitResult.TraceEnd;
		}
		const AWarCharacter* HitWarCharacter = Cast<AWarCharacter>(HitActor);
		if (!HitWarCharacter)
		{
			continue;
		}
		const UWarTeamManagerSubsystem* WarTeamManagerSubsystem = HitWarCharacter->GetWorld()->GetSubsystem<UWarTeamManagerSubsystem>();
		ensure(WarTeamManagerSubsystem);
		const float TeamDamage = WarTeamManagerSubsystem->CanDealDamage(GetOwner(), HitActor) ? 1.0f : 0.0f;
		double BulletFlyDistance = WORLD_MAX;
		BulletFlyDistance = FVector::Dist(TraceStart, ImpactLocation);
		const FRichCurve* DamageDistanceAttenuationCurve = WarWeaponPropertiesComponent->GetDamageDistanceAttenuationFloatCurve().GetRichCurveConst();
		float DamageDistanceAttenuation = 1.0f;
		if (DamageDistanceAttenuationCurve->HasAnyData())
		{
			DamageDistanceAttenuation = DamageDistanceAttenuationCurve->Eval(BulletFlyDistance);
		}
		float PhysicalMaterialDamage = 1.0f;
		const UPhysicalMaterial* PhysicalMaterial = HitResult.PhysMaterial.Get();
		const UWarPhysicalMaterial* WarPhysicalMaterial = Cast<const UWarPhysicalMaterial>(PhysicalMaterial);
		if (WarPhysicalMaterial)
		{
			for (const FGameplayTag PhysicalMaterialTag : WarPhysicalMaterial->GameplayTagContainer)
			{
				if (const float* PhysicalMaterialDamageMultiplier = WarWeaponPropertiesComponent->GetPhysicalMaterialDamageMap().Find(PhysicalMaterialTag))
				{
					PhysicalMaterialDamage *= (*PhysicalMaterialDamageMultiplier);
					// break;
				}
			}
		}
		const float Damage = FMath::Max(WarWeaponPropertiesComponent->GetBaseDamage() * DamageDistanceAttenuation * PhysicalMaterialDamage * TeamDamage, 0.0f);
		if (Damage > 0.0f)
		{
			UWarHealthComponent* WarHealthComponent = HitWarCharacter->FindComponentByClass<UWarHealthComponent>();
			WarHealthComponent->SetCurrentHealth(WarHealthComponent->GetCurrentHealth() - Damage);
		}
	}
}

void AWarWeapon::Reload()
{
	if (WarWeaponAmmunitionComponent->GetCurrentAmmunitionCapacity() <= 0)
	{
		WarWeaponStateComponent->SetCurrentWarWeaponState(EWarWeaponState::OutOfAmmunition);
	}
	else
	{
		bIsReloading = true;
		WarWeaponStateComponent->SetCurrentWarWeaponState(EWarWeaponState::Reloading);
		OnReloading();
	}
}

void AWarWeapon::OnFired()
{
	bIsFiring = false;
}

void AWarWeapon::OnReloaded()
{
	bIsReloading = false;
	const int32& CurrentMagazineCapacity = WarWeaponAmmunitionComponent->GetCurrentMagazineCapacity();
	const int32& CurrentAmmunitionCapacity = WarWeaponAmmunitionComponent->GetCurrentAmmunitionCapacity();
	const int32& MaxMagazineCapacity = WarWeaponAmmunitionComponent->GetMaxMagazineCapacity();
	const int32& ConsumedMagazine = MaxMagazineCapacity - CurrentAmmunitionCapacity;
	const int32& RemainingAmmunitionCapacity = CurrentAmmunitionCapacity - ConsumedMagazine;
	WarWeaponAmmunitionComponent->SetCurrentMagazineCapacity(MaxMagazineCapacity);
	WarWeaponAmmunitionComponent->SetCurrentAmmunitionCapacity(RemainingAmmunitionCapacity);
	WarWeaponStateComponent->SetCurrentWarWeaponState(EWarWeaponState::Ready);
}

bool AWarWeapon::PickupWeapon(APawn* PickingPawn)
{
	AWarCharacter* WarCharacter = Cast<AWarCharacter>(PickingPawn);
	check(WarCharacter);
	UWarInventoryComponent* WarInventoryComponent = Cast<UWarInventoryComponent>(WarCharacter->GetComponentByClass(UWarInventoryComponent::StaticClass()));
	check(WarInventoryComponent);

	if (const UWarInventoryItemInstance* WarInventoryItemInstance = WarInventoryComponent->FindItemByDefinition(ItemDefinition))
	{
		const AActor* OwnedItemActor = WarInventoryItemInstance->GetItemActor();
		check(OwnedItemActor);
		const AWarWeapon* OwnedWarWeapon = Cast<AWarWeapon>(OwnedItemActor);
		check(OwnedWarWeapon);
		UWarWeaponAmmunitionComponent* OwnedWarWeaponAmmunitionComponent = Cast<UWarWeaponAmmunitionComponent>(OwnedWarWeapon->GetComponentByClass(UWarWeaponAmmunitionComponent::StaticClass()));
		check(OwnedWarWeaponAmmunitionComponent);
		const int32& MaxAmmunitionCapacity = WarWeaponAmmunitionComponent->GetMaxAmmunitionCapacity();
		const int32& MaxMagazineCapacity = WarWeaponAmmunitionComponent->GetMaxMagazineCapacity();
		const int32& CurrentAmmunitionCapacity = WarWeaponAmmunitionComponent->GetCurrentAmmunitionCapacity();
		const int32& OwnedWarWeaponCurrentAmmunition = OwnedWarWeaponAmmunitionComponent->GetCurrentAmmunitionCapacity();
		const int32& AmmunitionLack = MaxAmmunitionCapacity - OwnedWarWeaponCurrentAmmunition;
		if (AmmunitionLack < CurrentAmmunitionCapacity)
		{
			WarWeaponAmmunitionComponent->SetCurrentAmmunitionCapacity(CurrentAmmunitionCapacity - AmmunitionLack);
			OwnedWarWeaponAmmunitionComponent->SetCurrentAmmunitionCapacity(MaxAmmunitionCapacity);
		}
		else
		{
			OwnedWarWeaponAmmunitionComponent->SetCurrentAmmunitionCapacity(OwnedWarWeaponCurrentAmmunition + CurrentAmmunitionCapacity);
			WarWeaponAmmunitionComponent->SetCurrentAmmunitionCapacity(0);
			Destroy();
		}
		return true;
	}
	else
	{
		TObjectPtr<UWarInventoryItemInstance> NewWarInventoryItemInstance = NewObject<UWarInventoryItemInstance>(WarInventoryComponent->GetOwner());
		NewWarInventoryItemInstance->SetItemDefinition(ItemDefinition);
		NewWarInventoryItemInstance->SetItemActor(this);
		if (WarInventoryComponent->IsWeaponSlotsFull())
		{
			WarInventoryComponent->ReplaceActiveWeapon(NewWarInventoryItemInstance);
		}
		else
		{
			WarInventoryComponent->AddItemInstance(NewWarInventoryItemInstance);
			WarInventoryComponent->AddWeaponToSlot(NewWarInventoryItemInstance);
			WarInventoryComponent->CycleActiveWeaponForward();
		}
		SetActorRelativeTransform(WeaponAttachTransform);
		AttachToComponent(WarCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSkeletalSocket);
		SetOwner(WarCharacter);
		if (!WarCharacter->IsArmed())
		{
			WarCharacter->SetIsArmed(true);
		}
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponSkeletalMesh->SetSimulatePhysics(false);
		WeaponSkeletalMesh->SetEnableGravity(false);
		WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WarWeaponStateComponent->SetCurrentWarWeaponState(EWarWeaponState::Ready);
		OnWeaponPickingUp();
		return true;
	}
	return false;
}

bool AWarWeapon::DropWeapon(APawn* DropingPawn)
{
	check(DropingPawn);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponSkeletalMesh->SetSimulatePhysics(true);
	WeaponSkeletalMesh->SetEnableGravity(true);
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	WeaponSkeletalMesh->MarkRenderStateDirty();
	SetOwner(nullptr);
	WarWeaponStateComponent->SetCurrentWarWeaponState(EWarWeaponState::NoOwner);
	return false;
}

// Called when the game starts or when spawned
void AWarWeapon::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWarWeapon::OnCollisionBeginSphereOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AWarWeapon::OnCollisionEndSphereOverplay);
}

void AWarWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (EndPlayReason == EEndPlayReason::Type::Destroyed)
	{
		CollisionSphere->OnComponentBeginOverlap.Clear();
		CollisionSphere->OnComponentEndOverlap.Clear();
	}
}

void AWarWeapon::Targeting(OUT TArray<FHitResult>& OutHitResults)
{
	APawn* const Pawn = Cast<APawn>(GetOwner());
	if (Pawn && Pawn->IsLocallyControlled())
	{
		FWarWeaponFireParameters WarWeaponFireParameters;
		const FTransform TargetTransform = GetTargetingTransform(Pawn);
		WarWeaponFireParameters.AimDirection = TargetTransform.GetUnitAxis(EAxis::X);
		WarWeaponFireParameters.TraceStart = TargetTransform.GetTranslation();
		WarWeaponFireParameters.AimEnd = WarWeaponFireParameters.TraceStart + WarWeaponFireParameters.AimDirection * WarWeaponPropertiesComponent->GetDealDamageMaxRange();
		TraceBulletsInShot(WarWeaponFireParameters, OutHitResults);
	}
}

FTransform AWarWeapon::GetTargetingTransform(APawn* Pawn) const
{
	check(Pawn)
	AController* Controller = Pawn->GetController();
	const FVector ActorLoation = Pawn->GetActorLocation();
	FQuat AimQuaternion = Pawn->GetActorQuat();
	FVector WeaponTargetingLocation;
	double FocalDistance = 1024.0f;
	FVector FocalLocation;
	FVector CameraLocation;
	FRotator CameraRotation;
	if (Controller != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (PlayerController != nullptr)
		{
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
		}
		else
		{
			WeaponTargetingLocation = GetWeaponTargetingLocation();
			CameraLocation = WeaponTargetingLocation;
			CameraRotation = Controller->GetControlRotation();
		}
		FVector AimDirection = CameraRotation.Vector().GetSafeNormal();
		FocalLocation = CameraLocation + (AimDirection * FocalDistance);

		if (PlayerController)
		{
			const FVector TargetingLocation = GetWeaponTargetingLocation();
			CameraLocation = FocalLocation + (((TargetingLocation - FocalLocation) | AimDirection) * AimDirection);
			FocalLocation = CameraLocation + (AimDirection * FocalDistance);
		}
		else if (AAIController* AIController = Cast<AAIController>(Controller))
		{
			CameraLocation = Pawn->GetActorLocation() + FVector(0, 0, Pawn->BaseEyeHeight);
		}
		return FTransform(CameraRotation, CameraLocation);
	}
	return FTransform(AimQuaternion, WeaponTargetingLocation);
}

FVector AWarWeapon::GetWeaponTargetingLocation() const
{
	APawn* const Pawn = Cast<APawn>(GetOwner());
	check(Pawn);
	const FVector ActorLocation = Pawn->GetActorLocation();
	const FQuat ActorRotation = Pawn->GetActorQuat();
	FVector WeaponTargetingLocation = ActorLocation;
	// ...
	return WeaponTargetingLocation;
}

void AWarWeapon::TraceBulletsInShot(const FWarWeaponFireParameters& WarWeaponParameter, OUT TArray<FHitResult>& OutHitResults)
{
	const int32 BulletPershot = WarWeaponPropertiesComponent->GetBulletsPerShot();
	for (int32 BulletIndex = 0; BulletIndex < BulletPershot; BulletIndex++)
	{
		const float BaseSpreadAngle = WarWeaponPropertiesComponent->GetCurrentSpreadAngle();
		const float SpreadAngleMultiplier = WarWeaponPropertiesComponent->GetCurrentSpreadAngleMultiplier();
		const float ActualSpreadAngle = BaseSpreadAngle * SpreadAngleMultiplier;
		const float HalfSpreadAngleInRadians = FMath::DegreesToRadians(ActualSpreadAngle * 0.5f);
		const FVector BulletDirection = VRandomConeNormalDistribution(WarWeaponParameter.AimDirection, HalfSpreadAngleInRadians, WarWeaponPropertiesComponent->GetFireSpreadExponent());
		const FVector TraceEnd = WarWeaponParameter.TraceStart + (BulletDirection * WarWeaponPropertiesComponent->GetDealDamageMaxRange());
		FVector HitLocation = TraceEnd;
		TArray<FHitResult> AllImpactsHitResults;
		FHitResult ImpactHitResult = TraceSingleBullet(WarWeaponParameter.TraceStart, TraceEnd, WarWeaponPropertiesComponent->GetBulletTraceSweepRadius(), false, AllImpactsHitResults);
		const AActor* HitActor = ImpactHitResult.GetActor();
		if (HitActor)
		{
			if (AllImpactsHitResults.Num() > 0)
			{
				OutHitResults.Append(AllImpactsHitResults);
			}
			HitLocation = ImpactHitResult.ImpactPoint;
		}
		if (OutHitResults.Num() == 0)
		{
			if (!ImpactHitResult.bBlockingHit)
			{
				ImpactHitResult.Location = TraceEnd;
				ImpactHitResult.ImpactPoint = TraceEnd;
			}
			OutHitResults.Add(ImpactHitResult);
		}
	}
}

FVector AWarWeapon::VRandomConeNormalDistribution(const FVector& Direction, const float ConeHalfAngleRad, const float Exponent)
{
	if (ConeHalfAngleRad > 0.0f)
	{
		const float ConeHalfAngleDegrees = FMath::RadiansToDegrees(ConeHalfAngleRad);
		const float FromCenter = FMath::Pow(FMath::FRand(), Exponent);
		const float AngleFromCenter = FromCenter * ConeHalfAngleDegrees;
		const float AngleAround = FMath::FRand() * 360.0f;
		FRotator Rotator = Direction.Rotation();
		FQuat DirectionQuaternion(Rotator);
		FQuat FromCenterQuaternion(FRotator(0.0f, AngleFromCenter, 0.0f));
		FQuat AroundQuaternion(FRotator(0.0f, 0.0, AngleAround));
		FQuat FinalDirectionQuaternion = DirectionQuaternion * AroundQuaternion * FromCenterQuaternion;
		FinalDirectionQuaternion.Normalize();
		return FinalDirectionQuaternion.RotateVector(FVector::ForwardVector);
	}
	else
	{
		return Direction.GetSafeNormal();
	}
}

FHitResult AWarWeapon::TraceSingleBullet(const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius, bool bIsSimulated, OUT TArray<FHitResult>& OutHitResults) const
{
	FHitResult ImpactHitResults;
	if (FindFirstPawnHitResult(OutHitResults) == INDEX_NONE)
	{
		ImpactHitResults = WeaponTrace(TraceStart, TraceEnd, 0.0f, bIsSimulated, OutHitResults);
	}
	if (FindFirstPawnHitResult(OutHitResults) == INDEX_NONE)
	{
		if (SweepRadius > 0.0f)
		{
			TArray<FHitResult> SweepHitResults;
			ImpactHitResults = WeaponTrace(TraceStart, TraceEnd, SweepRadius, bIsSimulated, SweepHitResults);
			const int32 FirstPawnIndex = FindFirstPawnHitResult(SweepHitResults);
			if (SweepHitResults.IsValidIndex(FirstPawnIndex))
			{
				bool bUseSweepHits = true;
				for (int32 Index = 0; Index < FirstPawnIndex; Index++)
				{
					const FHitResult& CurrentHitResult = SweepHitResults[Index];
					auto Predicate = [&CurrentHitResult](const FHitResult& Other)
					{
						return Other.HitObjectHandle == CurrentHitResult.HitObjectHandle;
					};
					if (CurrentHitResult.bBlockingHit && OutHitResults.ContainsByPredicate(Predicate))
					{
						bUseSweepHits = false;
						break;
					}
				}
				if (bUseSweepHits)
				{
					OutHitResults = SweepHitResults;
				}
			}
		}
	}
	return ImpactHitResults;
}

int32 AWarWeapon::FindFirstPawnHitResult(const TArray<FHitResult>& HitResults) const
{
	for (int32 Index = 0; Index < HitResults.Num(); Index++)
	{
		const FHitResult& CurrentHitResult = HitResults[Index];
		if (CurrentHitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()))
		{
			return Index;
		}
		else
		{
			AActor* Actor = CurrentHitResult.HitObjectHandle.FetchActor();
			if ((Actor != nullptr) && (Actor->GetAttachParentActor() != nullptr) && (Cast<APawn>(Actor->GetAttachParentActor()) != nullptr))
			{
				return Index;
			}
		}
	}
	return INDEX_NONE;
}

FHitResult AWarWeapon::WeaponTrace(const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionQueryParameters(SCENE_QUERY_STAT(WeaponTrace), true, GetOwner());
	CollisionQueryParameters.bReturnPhysicalMaterial = true;
	AddAdditionalTraceIgnoreActors(CollisionQueryParameters);
	const ECollisionChannel CollisionChannel = ECC_GameTraceChannel2;
	if (SweepRadius > 0.0f)
	{
		GetWorld()->SweepMultiByChannel(HitResults, TraceStart, TraceEnd, FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(SweepRadius), CollisionQueryParameters);
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, CollisionChannel, CollisionQueryParameters);
	}
	FHitResult HitResult(ForceInit);
	if (HitResults.Num() > 0)
	{
		for (FHitResult& CurrentHitResult : HitResults)
		{
			auto Predicate = [&CurrentHitResult](const FHitResult& Other)
			{
				return Other.HitObjectHandle == CurrentHitResult.HitObjectHandle;
			};
			if (!OutHitResults.ContainsByPredicate(Predicate))
			{
				OutHitResults.Add(CurrentHitResult);
			}
		}
		HitResult = OutHitResults.Last();
	}
	else
	{
		HitResult.TraceStart = TraceStart;
		HitResult.TraceEnd = TraceEnd;
	}
	return HitResult;
}

void AWarWeapon::AddAdditionalTraceIgnoreActors(FCollisionQueryParams& CollisionQueryParameters) const
{
	if (AActor * Actor = GetOwner())
	{
		TArray<AActor*> AttachedActors;
		Actor->GetAttachedActors(AttachedActors);
		CollisionQueryParameters.AddIgnoredActors(AttachedActors);
	}
}

void AWarWeapon::AddWeaponSpread()
{
	const float HeatPerShot = WarWeaponPropertiesComponent->GetHeatToHeatPerShotCurve().GetRichCurveConst()->Eval(WarWeaponPropertiesComponent->GetCurrentHeat());
	WarWeaponPropertiesComponent->SetCurrentHeat(ClampHeat(WarWeaponPropertiesComponent->GetCurrentHeat() + HeatPerShot));
	WarWeaponPropertiesComponent->SetCurrentSpreadAngle(WarWeaponPropertiesComponent->GetHeatToSpreadCurve().GetRichCurveConst()->Eval(WarWeaponPropertiesComponent->GetCurrentHeat()));
}

float AWarWeapon::ClampHeat(float NewHeat)
{
	float MinHeat, MaxHeat;
	ComputeHeatRange(MinHeat, MaxHeat);
	return FMath::Clamp(NewHeat, MinHeat, MaxHeat);
}

void AWarWeapon::ComputeHeatRange(float& MinHeat, float&MaxHeat)
{
	float Min1, Max1;
	WarWeaponPropertiesComponent->GetHeatToHeatPerShotCurve().GetRichCurveConst()->GetTimeRange(Min1, Max1);

	float Min2, Max2;
	WarWeaponPropertiesComponent->GetHeatToCoolDownPerSecondCurve().GetRichCurveConst()->GetTimeRange(Min2, Max2);

	float Min3, Max3;
	WarWeaponPropertiesComponent->GetHeatToSpreadCurve().GetRichCurveConst()->GetTimeRange(Min3, Max3);

	MinHeat = FMath::Min(FMath::Min(Min1, Min2), Min3);
	MaxHeat = FMath::Max(FMath::Max(Max1, Max2), Max3);
}

bool AWarWeapon::UpdateSpread(float DeltaTime)
{
	const float TimeSinceFired = GetWorld()->TimeSince(LastFireTime);
	if (TimeSinceFired > WarWeaponPropertiesComponent->GetSpreadRecoveryCoolDownDelay())
	{
		const float CoolDownRate = WarWeaponPropertiesComponent->GetHeatToCoolDownPerSecondCurve().GetRichCurveConst()->Eval(WarWeaponPropertiesComponent->GetCurrentHeat());
		WarWeaponPropertiesComponent->SetCurrentHeat(ClampHeat(WarWeaponPropertiesComponent->GetCurrentHeat() - (CoolDownRate * DeltaTime)));
		WarWeaponPropertiesComponent->SetCurrentSpreadAngle(WarWeaponPropertiesComponent->GetHeatToSpreadCurve().GetRichCurveConst()->Eval(WarWeaponPropertiesComponent->GetCurrentHeat()));
	}
	float MinSpread;
	float MaxSpread;
	WarWeaponPropertiesComponent->GetHeatToSpreadCurve().GetRichCurveConst()->GetValueRange(MinSpread, MaxSpread);
	return FMath::IsNearlyEqual(WarWeaponPropertiesComponent->GetCurrentSpreadAngle(), MinSpread, KINDA_SMALL_NUMBER);
}

bool AWarWeapon::UpdateMultipliers(float DeltaTime)
{
	const float MultiplierNearlyEqualThreshold = 0.05f;
	AWarCharacter* WarCharacter = Cast<AWarCharacter>(GetOwner());
	check(WarCharacter);
	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(WarCharacter->GetMovementComponent());

	const float WarCharacterSpeed = WarCharacter->GetVelocity().Size();
	const float WarCharacterMovementTargetValue = FMath::GetMappedRangeValueClamped(FVector2D(WarWeaponPropertiesComponent->GetStandingStillSpeedThreshold(), WarWeaponPropertiesComponent->GetStandingStillSpeedThreshold() + WarWeaponPropertiesComponent->GetStandingStillToMovingSpeedRange()), FVector2D(WarWeaponPropertiesComponent->GetStandingStillSpreadAngleMultiplier(), 1.0f), WarCharacterSpeed);
	WarWeaponPropertiesComponent->SetCurrentStandingStillMultiplier(FMath::FInterpTo(WarWeaponPropertiesComponent->GetCurrentStandingStillMultiplier(), WarCharacterMovementTargetValue, DeltaTime, WarWeaponPropertiesComponent->GetStandingStillTransitionRate()));
	const bool bStandingStillMultiplierAtMin = FMath::IsNearlyEqual(WarWeaponPropertiesComponent->GetCurrentStandingStillMultiplier(), WarWeaponPropertiesComponent->GetStandingStillSpreadAngleMultiplier(), WarWeaponPropertiesComponent->GetStandingStillSpreadAngleMultiplier() * 0.1f);

	const bool bIsCrouching = (CharacterMovementComponent !=nullptr) && CharacterMovementComponent->IsCrouching();
	const float CrouchingTargetValue = bIsCrouching ? WarWeaponPropertiesComponent->GetCrouchSpreadAngleMultiplier() : 1.0f;
	WarWeaponPropertiesComponent->SetCurrentCrouchMultiplier(FMath::FInterpTo(WarWeaponPropertiesComponent->GetCurrentCrouchMultiplier(), CrouchingTargetValue, DeltaTime, MultiplierNearlyEqualThreshold));
	const bool bCrouchingMultiplierAtTarget = FMath::IsNearlyEqual(WarWeaponPropertiesComponent->GetCurrentStandingStillMultiplier(), WarWeaponPropertiesComponent->GetStandingStillSpreadAngleMultiplier(), WarWeaponPropertiesComponent->GetStandingStillSpreadAngleMultiplier() * 0.1f);

	const bool bIsJumpingOrFalling = (CharacterMovementComponent != nullptr) && CharacterMovementComponent->IsFalling();
	const float JumpFallTargetValue = bIsJumpingOrFalling ? WarWeaponPropertiesComponent->GetJumpingOrFallingSpreadAngleMultiplier() : 1.0f;
	WarWeaponPropertiesComponent->SetCurrentJumpFallMultiplier(FMath::FInterpTo(WarWeaponPropertiesComponent->GetCurrentJumpFallMultiplier(), CrouchingTargetValue, DeltaTime, WarWeaponPropertiesComponent->GetJumpingOrFallingsTransitionRate()));
	const bool bJumpFallMultiplierIsOne = FMath::IsNearlyEqual(WarWeaponPropertiesComponent->GetCurrentJumpFallMultiplier(), 1.0f, MultiplierNearlyEqualThreshold);

	float AimingAlpha = 0.0f;
	if (const UWarCameraComponent* WarCameraComponent = UWarCameraComponent::FindCameraComponent(WarCharacter))
	{
		float TopWarCameraWeight;
		EWarCameraType TopWarCameraType;
		WarCameraComponent->GetBlendInfo(TopWarCameraWeight, TopWarCameraType);
		AimingAlpha = (TopWarCameraType == EWarCameraType::AimDownSight) ? TopWarCameraWeight : 0.0f;
	}
	const float AimingMultiplier = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(1.0f, WarWeaponPropertiesComponent->GetAimingSpreadAngleMultiplier()), AimingAlpha);
	const bool bAimingMultiplierAtTarget = FMath::IsNearlyEqual(AimingMultiplier, WarWeaponPropertiesComponent->GetAimingSpreadAngleMultiplier(), KINDA_SMALL_NUMBER);

	const float CombinedMultiplier = AimingMultiplier * WarWeaponPropertiesComponent->GetCurrentStandingStillMultiplier() * WarWeaponPropertiesComponent->GetCurrentCrouchMultiplier() * WarWeaponPropertiesComponent->GetCurrentJumpFallMultiplier();
	WarWeaponPropertiesComponent->SetCurrentSpreadAngleMultiplier(CombinedMultiplier);
	return bStandingStillMultiplierAtMin && bCrouchingMultiplierAtTarget && bJumpFallMultiplierIsOne && bAimingMultiplierAtTarget;
}

void AWarWeapon::OnCollisionBeginSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWarCharacter* WarCharacter = Cast<AWarCharacter>(OtherActor);
	check(WarCharacter)
	WarCharacter->AddOverlappedActor(this);
}

void AWarWeapon::OnCollisionEndSphereOverplay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWarCharacter* WarCharacter = Cast<AWarCharacter>(OtherActor);
	check(WarCharacter)
	WarCharacter->RemoveOverlappedActor(this);
}
