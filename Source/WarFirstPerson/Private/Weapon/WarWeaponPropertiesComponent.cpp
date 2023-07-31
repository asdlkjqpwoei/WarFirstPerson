// Personal Copyright
#include "Weapon/WarWeaponPropertiesComponent.h"

// Sets default values for this component's properties
UWarWeaponPropertiesComponent::UWarWeaponPropertiesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called every frame
void UWarWeaponPropertiesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWarWeaponPropertiesComponent::SetCurrentSpreadAngle(const float& NewCurrentSpreadAngle)
{
	CurrentSpreadAngle = NewCurrentSpreadAngle;
}

float UWarWeaponPropertiesComponent::GetCurrentSpreadAngle() const
{
	return CurrentSpreadAngle;
}

void UWarWeaponPropertiesComponent::SetCurrentHeat(const float& NewCurrentHeat)
{
	CurrentHeat = NewCurrentHeat;
}

float UWarWeaponPropertiesComponent::GetCurrentHeat() const
{
	return CurrentHeat;
}

void UWarWeaponPropertiesComponent::SetCurrentSpreadAngleMultiplier(const float& NewCurrentSpreadAngleMultiplier)
{
	CurrentSpreadAngleMultiplier = NewCurrentSpreadAngleMultiplier;
}

float UWarWeaponPropertiesComponent::GetCurrentSpreadAngleMultiplier() const
{
	return CurrentSpreadAngleMultiplier;
}

void UWarWeaponPropertiesComponent::SetCurrentStandingStillMultiplier(const float& NewCurrentStandingStillMultiplier)
{
	CurrentStandingStillMultiplier = NewCurrentStandingStillMultiplier;
}

float UWarWeaponPropertiesComponent::GetCurrentStandingStillMultiplier() const
{
	return CurrentStandingStillMultiplier;
}

void UWarWeaponPropertiesComponent::SetCurrentJumpFallMultiplier(const float& NewCurrentJumpFallMultiplier)
{
	CurrentJumpFallMultiplier = NewCurrentJumpFallMultiplier;
}

float UWarWeaponPropertiesComponent::GetCurrentJumpFallMultiplier() const
{
	return CurrentJumpFallMultiplier;
}

void UWarWeaponPropertiesComponent::SetCurrentCrouchMultiplier(const float& NewCurrentCrouchMultiplier)
{
	CurrentCrouchMultiplier = NewCurrentCrouchMultiplier;
}

float UWarWeaponPropertiesComponent::GetCurrentCrouchMultiplier() const
{
	return CurrentCrouchMultiplier;
}

void UWarWeaponPropertiesComponent::SetBaseDamage(const float& NewBaseDamage)
{
	BaseDamage = NewBaseDamage;
}

float UWarWeaponPropertiesComponent::GetBaseDamage() const
{
	return BaseDamage;
}

void UWarWeaponPropertiesComponent::SetDamageDistanceAttenuationFloatCurve(const FRuntimeFloatCurve& NewDamageDistanceAttenuationFloatCurve)
{
	DamageDistanceAttenuationFloatCurve = NewDamageDistanceAttenuationFloatCurve;
}

FRuntimeFloatCurve UWarWeaponPropertiesComponent::GetDamageDistanceAttenuationFloatCurve() const
{
	return DamageDistanceAttenuationFloatCurve;
}

void UWarWeaponPropertiesComponent::SetPhysicalMaterialDamageMap(const TMap<FGameplayTag, float> NewPhysicalMaterialDamageMap)
{
	PhysicalMaterialDamageMap = NewPhysicalMaterialDamageMap;
}

TMap<FGameplayTag, float> UWarWeaponPropertiesComponent::GetPhysicalMaterialDamageMap() const
{
	return PhysicalMaterialDamageMap;
}

void UWarWeaponPropertiesComponent::SetDealDamageMaxRange(const float& NewDealDamageMaxRange)
{
	DealDamageMaxRange = NewDealDamageMaxRange;
}

float UWarWeaponPropertiesComponent::GetDealDamageMaxRange() const
{
	return DealDamageMaxRange;
}

void UWarWeaponPropertiesComponent::SetBulletsPerShot(const int32& NewBulletsPerShot)
{
	BulletsPerShot = NewBulletsPerShot;
}

int32 UWarWeaponPropertiesComponent::GetBulletsPerShot() const
{
	return BulletsPerShot;
}

void UWarWeaponPropertiesComponent::SetAllowFirstShotAccracy(const bool& bNewAllowFirstShotAccuracy)
{
	bAllowFirstShotAccuracy = bNewAllowFirstShotAccuracy;
}

bool UWarWeaponPropertiesComponent::GetAllowFirstShotAccuracy() const
{
	return bAllowFirstShotAccuracy;
}

void UWarWeaponPropertiesComponent::SetBulletTraceSweepRadius(const float& NewBulletTraceSweepRadius)
{
	BulletTraceSweepRadius = NewBulletTraceSweepRadius;
}

float UWarWeaponPropertiesComponent::GetBulletTraceSweepRadius() const
{
	return BulletTraceSweepRadius;
}

void UWarWeaponPropertiesComponent::SetFireSpreadExponent(const float& NewFireSpreadExponent)
{
	FireSpreadExponent = NewFireSpreadExponent;
}

float UWarWeaponPropertiesComponent::GetFireSpreadExponent() const
{
	return FireSpreadExponent;
}

void UWarWeaponPropertiesComponent::SetMaterialDamageMultiplier(const TMap<FGameplayTag, float>& NewMaterialDamageMultiplier)
{
	MaterialDamageMultiplier = NewMaterialDamageMultiplier;
}

TMap<FGameplayTag, float> UWarWeaponPropertiesComponent::GetMaterialDamageMultiplier() const
{
	return MaterialDamageMultiplier;
}

void UWarWeaponPropertiesComponent::SetSpreadExponent(const float& NewSpreadExponent)
{
	SpreadExponent = NewSpreadExponent;
}

float UWarWeaponPropertiesComponent::GetSpreadExponent() const
{
	return SpreadExponent;
}

void UWarWeaponPropertiesComponent::SetHeatToSpreadCurve(const FRuntimeFloatCurve& NewHeatToSpreadCurve)
{
	HeatToHeatPerShotCurve = NewHeatToSpreadCurve;
}

FRuntimeFloatCurve UWarWeaponPropertiesComponent::GetHeatToSpreadCurve() const
{
	return HeatToHeatPerShotCurve;
}

void UWarWeaponPropertiesComponent::SetHeatToHeatPerShotCurve(const FRuntimeFloatCurve& NewHeatToHeatPerShotCurve)
{
	HeatToHeatPerShotCurve = NewHeatToHeatPerShotCurve;
}

FRuntimeFloatCurve UWarWeaponPropertiesComponent::GetHeatToHeatPerShotCurve() const
{
	return HeatToHeatPerShotCurve;
}

void UWarWeaponPropertiesComponent::SetHeatToCoolDownPerSecondCurve(const FRuntimeFloatCurve& NewHeatToCoolDownPerSecondCurve)
{
	HeatToCoolDownPerSecondCurve = NewHeatToCoolDownPerSecondCurve;
}

FRuntimeFloatCurve UWarWeaponPropertiesComponent::GetHeatToCoolDownPerSecondCurve() const
{
	return HeatToCoolDownPerSecondCurve;
}

void UWarWeaponPropertiesComponent::SetAimingSpreadAngleMultiplier(const float& NewAimingSpreadAngleMultiplier)
{
	AimingSpreadAngleMultiplier = NewAimingSpreadAngleMultiplier;
}

float UWarWeaponPropertiesComponent::GetAimingSpreadAngleMultiplier() const
{
	return AimingSpreadAngleMultiplier;
}

void UWarWeaponPropertiesComponent::SetStandingStillSpreadAngleMultiplier(const float& NewStandingStillSpreadAngleMultiplier)
{
	StandingStillSpreadAngleMultiplier = NewStandingStillSpreadAngleMultiplier;
}

float UWarWeaponPropertiesComponent::GetStandingStillSpreadAngleMultiplier() const
{
	return StandingStillSpreadAngleMultiplier;
}

void UWarWeaponPropertiesComponent::SetStandingStillTransitionRate(const float& NewStandingStillTransitionRate)
{
	StandingStillTransitionRate = NewStandingStillTransitionRate;
}

float UWarWeaponPropertiesComponent::GetStandingStillTransitionRate() const
{
	return StandingStillTransitionRate;
}

void UWarWeaponPropertiesComponent::SetStandingStillSpeedThreshold(const float& NewStandingStillSpeedThreshold)
{
	StandingStillSpeedThreshold = NewStandingStillSpeedThreshold;
}

float UWarWeaponPropertiesComponent::GetStandingStillSpeedThreshold() const
{
	return StandingStillSpeedThreshold;
}

void UWarWeaponPropertiesComponent::SetStandingStillToMovingSpeedRange(const float& NewStandingStillToMovingSpeedRange)
{
	StandingStillToMovingSpeedRange = NewStandingStillToMovingSpeedRange;
}

float UWarWeaponPropertiesComponent::GetStandingStillToMovingSpeedRange() const
{
	return StandingStillToMovingSpeedRange;
}

void UWarWeaponPropertiesComponent::SetCrouchSpreadAngleMultiplier(const float& NewCrouchSpreadAngleMultiplier)
{
	CrouchSpreadAngleMultiplier = NewCrouchSpreadAngleMultiplier;
}

float UWarWeaponPropertiesComponent::GetCrouchSpreadAngleMultiplier() const
{
	return CrouchSpreadAngleMultiplier;
}

void UWarWeaponPropertiesComponent::SetCrouchTransitionRate(const float& NewCrouchTransitionRate)
{
	CrouchTransitionRate = NewCrouchTransitionRate;
}

float UWarWeaponPropertiesComponent::GetCrouchTransitionRate() const
{
	return CrouchTransitionRate;
}

void UWarWeaponPropertiesComponent::SetJumpingOrFallingSpreadAngleMultiplier(const float& NewJumpingOrFallingSpreadAngleMultiplier)
{
	JumpingOrFallingSpreadAngleMultiplier = NewJumpingOrFallingSpreadAngleMultiplier;
}

float UWarWeaponPropertiesComponent::GetJumpingOrFallingSpreadAngleMultiplier() const
{
	return JumpingOrFallingSpreadAngleMultiplier;
}

// Called when the game starts
void UWarWeaponPropertiesComponent::BeginPlay()
{
	Super::BeginPlay();
}
