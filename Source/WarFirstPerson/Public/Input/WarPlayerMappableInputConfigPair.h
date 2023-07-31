// Personal Copyright
#pragma once
#include "CommonInputBaseTypes.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WarPlayerMappableInputConfigPair.generated.h"

class UPlayerMappableInputConfig;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FLoadedMappableConfigPair
{
	GENERATED_BODY()
	FLoadedMappableConfigPair() = default;
	FLoadedMappableConfigPair(const UPlayerMappableInputConfig* InConfig, ECommonInputType InType, const bool InIsActive) : PlayerMappableInputConfig(InConfig), CommonInputType(InType), bIsActive(InIsActive)
	{

	}
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<const UPlayerMappableInputConfig> PlayerMappableInputConfig = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ECommonInputType CommonInputType = ECommonInputType::Count;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsActive = false;
};

USTRUCT(BlueprintType)
struct FWarPlayerMappableInputConfigPair
{
	GENERATED_BODY()
	FWarPlayerMappableInputConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> PlayerMappableInputConfig;

	UPROPERTY(EditAnywhere)
	ECommonInputType CommonInputType = ECommonInputType::Count;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer DependentPlatformTraits;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer ExcludePlatformTraits;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;

	bool CanBeActivated() const;
	static bool RegisterPair(const FWarPlayerMappableInputConfigPair& WarPlayerMappableConfigPair);
	static void UnregisterPair(const FWarPlayerMappableInputConfigPair& WarPlayerMappableConfigPair);
};
