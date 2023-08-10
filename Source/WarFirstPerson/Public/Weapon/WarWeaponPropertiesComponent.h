// Personal Copyright
#pragma once
#include "Components/GameFrameworkComponent.h"
#include "GameplayTagContainer.h"
#include "WarWeaponPropertiesComponent.generated.h"

UCLASS()
class WARFIRSTPERSON_API UWarWeaponPropertiesComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWarWeaponPropertiesComponent(const FObjectInitializer& ObjectInitializer);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCurrentSpreadAngle(const float& NewCurrentSpreadAngle);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	float GetCurrentSpreadAngle() const;

	void SetCurrentHeat(const float& NewCurrentHeat);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	float GetCurrentHeat() const;

	void SetCurrentSpreadAngleMultiplier(const float& NewCurrentSpreadAngleMultiplier);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	float GetCurrentSpreadAngleMultiplier() const;

	void SetCurrentStandingStillMultiplier(const float& NewCurrentStandingStillMultiplier);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	float GetCurrentStandingStillMultiplier() const;

	void SetCurrentJumpFallMultiplier(const float& NewCurrentJumpFallMultiplier);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	float GetCurrentJumpFallMultiplier() const;

	void SetCurrentCrouchMultiplier(const float& NewCurrentCrouchMultiplier);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	float GetCurrentCrouchMultiplier() const;

	void SetBaseDamage(const float& NewBaseDamage);
	
	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	float GetBaseDamage() const;

	void SetDamageDistanceAttenuationFloatCurve(const FRuntimeFloatCurve& NewDamageDistanceAttenuationFloatCurve);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	FRuntimeFloatCurve GetDamageDistanceAttenuationFloatCurve() const;

	void SetPhysicalMaterialDamageMap(const TMap<FGameplayTag, float> NewPhysicalMaterialDamageMap);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	TMap<FGameplayTag, float> GetPhysicalMaterialDamageMap() const;

	void SetDealDamageMaxRange(const float& NewDealDamageMaxRange);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	float GetDealDamageMaxRange() const;

	void SetBulletsPerShot(const int32& NewBulletsPerShot);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	int32 GetBulletsPerShot() const;

	void SetAllowFirstShotAccracy(const bool& bNewAllowFirstShotAccuracy);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	bool GetAllowFirstShotAccuracy() const;

	void SetBulletTraceSweepRadius(const float& NewBulletTraceSweepRadius);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	float GetBulletTraceSweepRadius() const;

	void SetFireSpreadExponent(const float& NewFireSpreadExponent);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	float GetFireSpreadExponent() const;

	void SetSpreadRecoveryCoolDownDelay(const float NewSpreadRecoveryCoolDownDelay);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	float GetSpreadRecoveryCoolDownDelay() const;

	void SetMaterialDamageMultiplier(const TMap<FGameplayTag, float>& NewMaterialDamageMultiplier);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	TMap<FGameplayTag, float> GetMaterialDamageMultiplier() const;

	void SetHeatToSpreadCurve(const FRuntimeFloatCurve& NewHeatToSpreadCurve);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	FRuntimeFloatCurve GetHeatToSpreadCurve() const;

	void SetHeatToHeatPerShotCurve(const FRuntimeFloatCurve& NewHeatToHeatPerShotCurve);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	FRuntimeFloatCurve GetHeatToHeatPerShotCurve() const;

	void SetHeatToCoolDownPerSecondCurve(const FRuntimeFloatCurve& NewHeatCoolDownPerSecondCurve);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Properties")
	FRuntimeFloatCurve GetHeatToCoolDownPerSecondCurve() const;

	void SetAimingSpreadAngleMultiplier(const float& NewAimingSpreadAngleMultiplier);

	UFUNCTION(BlueprintCallable, Category = "War|Character Properties")
	float GetAimingSpreadAngleMultiplier() const;

	void SetStandingStillSpreadAngleMultiplier(const float& NewStandingStillSpreadAngleMultiplier);

	UFUNCTION(BlueprintCallable, Category = "War|Character Properties")
	float GetStandingStillSpreadAngleMultiplier() const;

	void SetStandingStillTransitionRate(const float& NewStandingStillTransitionRate);

	UFUNCTION(BlueprintCallable, Category = "War|Character Properties")
	float GetStandingStillTransitionRate() const;

	void SetStandingStillSpeedThreshold(const float& NewStandingStillSpeedThreshold);

	UFUNCTION(BlueprintCallable, Category = "War|Character Properties")
	float GetStandingStillSpeedThreshold() const;

	void SetStandingStillToMovingSpeedRange(const float& NewStandingStillToMovingSpeedRange);

	UFUNCTION(BlueprintCallable, Category = "War|Character Properties")
	float GetStandingStillToMovingSpeedRange() const;

	void SetCrouchSpreadAngleMultiplier(const float& NewCrouchSpreadAngleMultiplier);

	UFUNCTION(BlueprintCallable, Category = "War|Character Properties")
	float GetCrouchSpreadAngleMultiplier() const;

	void SetCrouchTransitionRate(const float& NewCrouchTransitionRate);

	UFUNCTION(BlueprintCallable, Category = "War|Character Properties")
	float GetCrouchTransitionRate() const;

	void SetJumpingOrFallingSpreadAngleMultiplier(const float& NewJumpingOrFallingSpreadAngleMultiplier);

	UFUNCTION(BlueprintCallable, Category = "War|Character Properties")
	float GetJumpingOrFallingSpreadAngleMultiplier() const;
	
	void SetJumpingOrFallingsTransitionRate(const float& NewJumpingOrFallingsTransitionRate);

	UFUNCTION(BlueprintCallable, Category = "War|Character Properties")
	float GetJumpingOrFallingsTransitionRate() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "War|Weapon Properties", BlueprintGetter = GetCurrentSpreadAngle)
	float CurrentSpreadAngle = 0.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "War|Weapon Properties", BlueprintGetter = GetCurrentHeat)
	float CurrentHeat = 0.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "War|Weapon Properties", BlueprintGetter = GetCurrentSpreadAngleMultiplier)
	float CurrentSpreadAngleMultiplier = 0.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "War|Weapon Properties", BlueprintGetter = GetCurrentStandingStillMultiplier)
	float CurrentStandingStillMultiplier = 0.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "War|Weapon Properties", BlueprintGetter = GetCurrentJumpFallMultiplier)
	float CurrentJumpFallMultiplier = 0.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "War|Weapon Properties", BlueprintGetter = GetCurrentCrouchMultiplier)
	float CurrentCrouchMultiplier = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetBaseDamage)
	float BaseDamage = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetDamageDistanceAttenuationFloatCurve, Meta = (ForceUnits = cm))
	FRuntimeFloatCurve DamageDistanceAttenuationFloatCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetPhysicalMaterialDamageMap)
	TMap<FGameplayTag, float> PhysicalMaterialDamageMap;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetDealDamageMaxRange)
	float DealDamageMaxRange = 10000.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetBulletsPerShot)
	int32 BulletsPerShot = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetAllowFirstShotAccuracy)
	bool bAllowFirstShotAccuracy = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetBulletTraceSweepRadius)
	float BulletTraceSweepRadius = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetFireSpreadExponent, Meta = (ClampMin = 0.1))
	float FireSpreadExponent = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetSpreadRecoveryCoolDownDelay, Meta = (ForceUnits = s))
	float SpreadRecoveryCoolDownDelay = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetMaterialDamageMultiplier)
	TMap<FGameplayTag, float> MaterialDamageMultiplier;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetHeatToSpreadCurve)
	FRuntimeFloatCurve HeatToSpreadCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetHeatToHeatPerShotCurve)
	FRuntimeFloatCurve HeatToHeatPerShotCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Properties", BlueprintGetter = GetHeatToCoolDownPerSecondCurve)
	FRuntimeFloatCurve HeatToCoolDownPerSecondCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Character Properties", BlueprintGetter = GetAimingSpreadAngleMultiplier, Meta = (ForceUnits = x))
	float AimingSpreadAngleMultiplier = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Character Properties", BlueprintGetter = GetStandingStillSpreadAngleMultiplier, Meta = (ForceUnits = x))
	float StandingStillSpreadAngleMultiplier = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Character Properties", BlueprintGetter = GetStandingStillTransitionRate)
	float StandingStillTransitionRate = 5.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Character Properties", BlueprintGetter = GetStandingStillSpeedThreshold, Meta = (ForceUnits = "cm/s"))
	float StandingStillSpeedThreshold = 80.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Character Properties", BlueprintGetter = GetStandingStillToMovingSpeedRange, Meta = (ForceUnits = "cm/s"))
	float StandingStillToMovingSpeedRange = 20.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Character Properties", BlueprintGetter = GetCrouchSpreadAngleMultiplier, Meta = (ForceUnits = x))
	float CrouchSpreadAngleMultiplier = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Character Properties", BlueprintGetter = GetCrouchTransitionRate)
	float CrouchTransitionRate = 5.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Character Properties", BlueprintGetter = GetJumpingOrFallingSpreadAngleMultiplier, Meta = (ForceUnits = x))
	float JumpingOrFallingSpreadAngleMultiplier = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Character Properties", BlueprintGetter = GetJumpingOrFallingsTransitionRate)
	float JumpingOrFallingTransitionRate = 5.0f;
};
