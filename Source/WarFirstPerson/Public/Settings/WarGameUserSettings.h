// Personal Copyright

#pragma once

#include "CommonInputBaseTypes.h"
#include "Containers/Map.h"
#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "WarGameUserSettings.generated.h"

class UPlayerMappableInputConfig;
struct FLoadedMappableConfigPair;

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API UWarGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	UWarGameUserSettings();
	static UWarGameUserSettings* Get();
	virtual float GetEffectiveFrameRateLimit() override;
	virtual void SetToDefaults() override;

	UFUNCTION()
	float GetDisplayGamma() const;
	UFUNCTION()
	void SetDisplayGamma(float InGamma);

	UFUNCTION()
	float GetFrameRateLimitOnBattery() const;
	UFUNCTION()
	void SetFrameRateLimitOnBattery(float NewLimitFPS);

	UFUNCTION()
	float GetFrameRateLimitInMenu() const;
	UFUNCTION()
	void SetFrameRateLimitInMenu(float NewLimitFPS);

	UFUNCTION()
	float GetFrameRateLimitWhenBackgrounded() const;
	UFUNCTION()
	void SetFrameRateLimitWhenBackgrounded(float NewLimitFPS);

	UFUNCTION()
	float GetFrameRateLimitAlways() const;
	UFUNCTION()
	void SetFrameRateLimitAlways(float NewLimitFPS);

	const TMap<FName, FKey>& GetCustomPlayerInputConfig() const { return CustomKeyboardConfig; }

	void RegisterInputConfig(ECommonInputType Type, const UPlayerMappableInputConfig* NewConfig, const bool bIsActive);

protected:
	void ApplyDisplayGamma();
	void UpdateEffectiveFrameRateLimit();

private:
	UPROPERTY(Config)
	float DisplayGamma = 2.0f;

	UPROPERTY(Config)
	float FrameRateLimitOnBattery;
	UPROPERTY(Config)
	float FrameRateLimitInMenu;
	UPROPERTY(Config)
	float FrameRateLimitWhenBackgrounded;

	UPROPERTY(Config)
	FName InputConfigName = TEXT("Defaults");
	UPROPERTY(VisibleAnywhere)
	TArray<FLoadedMappableConfigPair> RegisteredInputConfigs;
	UPROPERTY(Config)
	TMap<FName, FKey> CustomKeyboardConfig;

	FDelegateHandle OnApplicationActivationStateChangedHandle;
};
