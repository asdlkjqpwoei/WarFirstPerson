// Personal Copyright
#include "WarFirstPersonEditorModule.h"
#include "IAssetTools.h"
#include "WarContextEffectsLibraryAssetTypeActions.h"

void FWarEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FWarContextEffectsLibraryAssetTypeActions));
}

void FWarEditorModule::ShutdownModule()
{
	FModuleManager::Get().OnModulesChanged().RemoveAll(this);
}

IMPLEMENT_MODULE(FWarEditorModule, WarFirstPersonEditor);