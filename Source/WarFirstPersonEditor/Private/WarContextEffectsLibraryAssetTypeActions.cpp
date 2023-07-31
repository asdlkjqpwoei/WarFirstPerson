// Personal Copyright
#include "WarContextEffectsLibraryAssetTypeActions.h"
#include "ContextEffect/WarContextEffectsLibrary.h"

class UClass;

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText FWarContextEffectsLibraryAssetTypeActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActiosn", "WarContextEffectsLibraryAssetTypeActions", "WarContextEffectsLibrary");
}

FColor FWarContextEffectsLibraryAssetTypeActions::GetTypeColor() const
{
	return FColor(65, 200, 98);
}

UClass* FWarContextEffectsLibraryAssetTypeActions::GetSupportedClass() const
{
	return UWarContextEffectsLibrary::StaticClass();
}

uint32 FWarContextEffectsLibraryAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Gameplay;
}
#undef LOCTEXT_NAMESPACE