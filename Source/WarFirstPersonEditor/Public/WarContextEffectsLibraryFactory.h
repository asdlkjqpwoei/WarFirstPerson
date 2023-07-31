// Personal Copyright
#pragma once
#include "Factories/Factory.h"
#include "WarContextEffectsLibraryFactory.generated.h"

class FFeedbackContext;
class UClass;
class UObject;

UCLASS(hidecategories = Object, MinimalAPI)
class UWarContextEffectsLibraryFactory : public UFactory
{
    GENERATED_UCLASS_BODY()
    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
    virtual bool ShouldShowInNewMenu() const override;
};