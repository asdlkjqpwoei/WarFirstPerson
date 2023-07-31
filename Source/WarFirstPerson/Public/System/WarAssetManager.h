// Personal Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/DataAsset.h"
#include "WarAssetManager.generated.h"

class UWarPawnData;

/**
 * 
 */
UCLASS(Config = Game)
class WARFIRSTPERSON_API UWarAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	UWarAssetManager();
	static UWarAssetManager& Get();

	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssertPointer, bool bKeepInMemory = true);

	const UWarPawnData* GetDefaultWarPawnData() const;

protected:
	virtual void StartInitialLoading() override;

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
	static bool ShouldLogAssetLoads();

	void AddLoadedAsset(const UObject* Asset);

	UPROPERTY(Config)
	TSoftObjectPtr<UWarPawnData> DefaultWarPawnData;

private:
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	FCriticalSection LoadedAssetsCritical;
};

template<typename AssetType>
inline AssetType* UWarAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}
		if (LoadedAsset && bKeepInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}
	return LoadedAsset;
}
