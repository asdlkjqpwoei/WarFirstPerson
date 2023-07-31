// Personal Copyright
#pragma once
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "WarInventoryItemDefinition.generated.h"

class AWarWeapon;
class UWarInventoryItemInstance;

UCLASS(Blueprintable, Const, Abstract)
class WARFIRSTPERSON_API UWarInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	UWarInventoryItemDefinition(const FObjectInitializer& Object = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Definition")
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Definition")
	FGameplayTag InventoryItemType;
};
