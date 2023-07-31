// Personal Copyright
#include "System/WarAssetManager.h"
#include "Character/WarPawnData.h"
#include "WarFirstPerson/WarLogChannels.h"
#include "WarFirstPerson/WarGameplayTags.h"

UWarAssetManager::UWarAssetManager() : DefaultWarPawnData(nullptr)
{
	
}

UWarAssetManager& UWarAssetManager::Get()
{
	check(GEngine);
	if (UWarAssetManager* WarAssetManagerSingleton = Cast<UWarAssetManager>(GEngine->AssetManager))
	{
		return *WarAssetManagerSingleton;
	}
	UE_LOG(LogWar, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini. It must be set to WarAssetManager!"));
	return *NewObject<UWarAssetManager>();
}

void UWarAssetManager::StartInitialLoading()
{
	SCOPED_BOOT_TIMING("UWarAssetManager::StartInitialLoading");
	Super::StartInitialLoading();
	FWarGameplayTags::InitializeNativeTags();
}

UObject* UWarAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePointer;
		if (ShouldLogAssetLoads())
		{
			LogTimePointer = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronous loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}
		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}
		return AssetPath.TryLoad();
	}
	return nullptr;
}

bool UWarAssetManager::ShouldLogAssetLoads()
{
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

void UWarAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}

const UWarPawnData* UWarAssetManager::GetDefaultWarPawnData() const
{
	return GetAsset(DefaultWarPawnData);
}

