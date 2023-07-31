// Personal Copyright
#include "WarContextEffectsLibraryFactory.h"
#include "ContextEffect/WarContextEffectsLibrary.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(WarContextEffectsLibraryFactory)

class FFeedbackContext;
class UClass;
class UObject;

UWarContextEffectsLibraryFactory::UWarContextEffectsLibraryFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UWarContextEffectsLibrary::StaticClass();
    bCreateNew = true;
    bEditorImport = false;
    bEditAfterNew = true;
}

UObject* UWarContextEffectsLibraryFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UWarContextEffectsLibrary* WarContextEffectsLibrary = NewObject<UWarContextEffectsLibrary>(InParent, Name, Flags);
    return WarContextEffectsLibrary;
}

bool UWarContextEffectsLibraryFactory::ShouldShowInNewMenu() const
{
    return true;
}