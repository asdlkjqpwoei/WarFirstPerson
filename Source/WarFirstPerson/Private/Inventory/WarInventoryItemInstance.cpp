// Personal Copyright

#include "Inventory/WarInventoryItemInstance.h"
#include "Inventory/WarInventoryItemDefinition.h"

UWarInventoryItemInstance::UWarInventoryItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

TSubclassOf<UWarInventoryItemDefinition> UWarInventoryItemInstance::GetItemDefinition() const
{
	return ItemDefinition;
}

APawn* UWarInventoryItemInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Pawn = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Pawn = Cast<APawn>(GetOuter());
		}
	}
	return Pawn;
}

void UWarInventoryItemInstance::OnEquipped()
{
	BP_OnEquipped();
}

void UWarInventoryItemInstance::OnUnequipped()
{
	BP_OnUnequipped();
}

void UWarInventoryItemInstance::SetItemDefinition(TSubclassOf<UWarInventoryItemDefinition> InItemDefinition)
{
	ItemDefinition = InItemDefinition;
}