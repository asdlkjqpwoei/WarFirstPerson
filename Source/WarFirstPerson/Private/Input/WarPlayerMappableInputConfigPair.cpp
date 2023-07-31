// Personal Copyright
#include "Input/WarPlayerMappableInputConfigPair.h"
#include "ICommonUIModule.h"
#include "CommonUISettings.h"
#include "PlayerMappableInputConfig.h"
#include "Settings/WarGameUserSettings.h"
#include "System/WarAssetManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(WarPlayerMappableInputConfigPair)

bool FWarPlayerMappableInputConfigPair::CanBeActivated() const
{
	const FGameplayTagContainer& PlatformTraits= ICommonUIModule::GetSettings().GetPlatformTraits();
	if (!DependentPlatformTraits.IsEmpty() && !PlatformTraits.HasAll(DependentPlatformTraits))
	{
		return false;
	}
	if (!ExcludePlatformTraits.IsEmpty() && PlatformTraits.HasAny(ExcludePlatformTraits))
	{
		return false;
	}
	return true;
}

bool FWarPlayerMappableInputConfigPair::RegisterPair(const FWarPlayerMappableInputConfigPair& WarPlayerMappableConfigPair)
{
	UWarAssetManager& AssetManager = UWarAssetManager::Get();
	if (UWarGameUserSettings* Settings = UWarGameUserSettings::Get())
	{
		if(const UPlayerMappableInputConfig* LoadedConfig = AssetManager.GetAsset(WarPlayerMappableConfigPair.PlayerMappableInputConfig))
		{
			Settings->RegisterInputConfig(WarPlayerMappableConfigPair.CommonInputType, LoadedConfig, false);
			return true;
		}
	}
	return false;
}

void FWarPlayerMappableInputConfigPair::UnregisterPair(const FWarPlayerMappableInputConfigPair& WarPlayerMappableConfigPair)
{
}
