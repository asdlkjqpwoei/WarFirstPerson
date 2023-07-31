// Personal Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WarInventoryItemInstance.generated.h"

class UWarInventoryItemDefinition;
struct FWarInventoryList;

UCLASS(BlueprintType, Blueprintable)
class WARFIRSTPERSON_API UWarInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UWarInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void SetItemDefinition(TSubclassOf<UWarInventoryItemDefinition> InItemDefinition);
	TSubclassOf<UWarInventoryItemDefinition> GetItemDefinition() const;
	virtual void OnEquipped();
	virtual void OnUnequipped();

	UFUNCTION(BlueprintPure, Category = "War|Inventory", meta = (DeterminesOutputType = PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "War|Inventory", meta = (DisplayName = "OnEquipped"))
	void BP_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = "War|Inventory", meta = (DisplayName = "OnUnequipped"))
	void BP_OnUnequipped();

private:

	friend FWarInventoryList;
	TSubclassOf<UWarInventoryItemDefinition> ItemDefinition;
};
