// Personal Copyright

#include "Inventory/WarInventoryComponent.h"
#include "Inventory/WarInventoryItemDefinition.h"
#include "Inventory/WarInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WarInventoryComponent)

FString FWarInventoryEntry::GetDebugString() const
{
	TSubclassOf<UWarInventoryItemDefinition> ItemDefinition;
	if (ItemInstance != nullptr)
	{
		ItemDefinition = ItemInstance->GetItemDefinition();
	}
	return FString::Printf(TEXT("%s (%d x &s)"), *GetNameSafe(ItemInstance), StackCount, *GetNameSafe(ItemDefinition));
}

void FWarInventoryList::AddEntry(UWarInventoryItemInstance* WarItemInstance, int32 StackCount)
{
	UWarInventoryItemInstance* Result = nullptr;
	check(WarItemInstance != nullptr);

	FWarInventoryEntry& NewEntry = WarInventoryEntries.AddDefaulted_GetRef();
	NewEntry.ItemInstance = WarItemInstance;
	NewEntry.StackCount = StackCount;
	Result = NewEntry.ItemInstance;
}

void FWarInventoryList::RemoveEntry(UWarInventoryItemInstance* ItemInstance)
{
	for (auto EntryIterator = WarInventoryEntries.CreateIterator(); EntryIterator; ++EntryIterator)
	{
		FWarInventoryEntry& WarInventoryEntry = *EntryIterator;
		if (WarInventoryEntry.ItemInstance == ItemInstance)
		{
			EntryIterator.RemoveCurrent();
		}
	}
}

TArray<UWarInventoryItemInstance*> FWarInventoryList::GetAllItems() const
{
	TArray<UWarInventoryItemInstance*> Results;
	Results.Reserve(WarInventoryEntries.Num());
	for (const FWarInventoryEntry& WarInventoryEntry : WarInventoryEntries)
	{
		if (WarInventoryEntry.ItemInstance != nullptr)
		{
			Results.Add(WarInventoryEntry.ItemInstance);
		}
	}
	return Results;
}

// Sets default values for this component's properties
UWarInventoryComponent::UWarInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), WarInventoryList(this)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UWarInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWarInventoryComponent::UnequipWeaponInSlot(int32 WeaponIndex)
{
	WarInventoryList.WarWeaponSlots[WeaponIndex]->OnUnequipped();
}

void UWarInventoryComponent::EquipWeaponInSlot(int32 WeaponIndex)
{
	WarInventoryList.WarWeaponSlots[WeaponIndex]->OnEquipped();
}

// Called every frame
void UWarInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UWarInventoryItemInstance* UWarInventoryComponent::FindItemByDefinition(TSubclassOf<UWarInventoryItemDefinition> ItemDefinition) const
{
	for (const FWarInventoryEntry& Entry : WarInventoryList.WarInventoryEntries)
	{
		UWarInventoryItemInstance* ItemInstance = Entry.ItemInstance;
		if (IsValid(ItemInstance))
		{
			if (ItemInstance->GetItemDefinition() == ItemDefinition)
			{
				return ItemInstance;
			}
		}
	}
	return nullptr;
}

void UWarInventoryComponent::AddItemInstance(UWarInventoryItemInstance* WarItemInstance, int32 StackCount)
{
	WarInventoryList.AddEntry(WarItemInstance);
}

void UWarInventoryComponent::RemoveItemInstance(UWarInventoryItemInstance* WarItemInstance)
{
	WarInventoryList.RemoveEntry(WarItemInstance);
}

TArray<UWarInventoryItemInstance*> UWarInventoryComponent::GetAllItems() const
{
	return WarInventoryList.GetAllItems();
}

void UWarInventoryComponent::CycleActiveWeaponForward()
{
	if (WarInventoryList.WarWeaponSlots.Num() < 2)
	{
		return ;
	}
	const int32 OldIndex = ActiveWeaponSlotIndex;
	int32 NewIndex = (ActiveWeaponSlotIndex + 1) % WarInventoryList.WarWeaponSlots.Num();
	if (NewIndex != OldIndex && WarInventoryList.WarWeaponSlots[NewIndex] != nullptr)
	{
		SetActiveSlotIndex(NewIndex);
	}
}

void UWarInventoryComponent::CycleActiveWeaponBackward()
{
	if (WarInventoryList.WarWeaponSlots.Num() < 2)
	{
		return ;
	}
	const int32 OldIndex = ActiveWeaponSlotIndex;
	int32 NewIndex = (ActiveWeaponSlotIndex - 1 + WarInventoryList.WarWeaponSlots.Num()) % WarInventoryList.WarWeaponSlots.Num();
	if (NewIndex != OldIndex && WarInventoryList.WarWeaponSlots[NewIndex] != nullptr)
	{
		SetActiveSlotIndex(NewIndex);
	}
}

TArray<UWarInventoryItemInstance*> UWarInventoryComponent::GetSlots() const
{
	return WarInventoryList.WarWeaponSlots;
}

bool UWarInventoryComponent::IsWeaponSlotsFull() const
{
	if (WarInventoryList.WarWeaponSlots.Num() < WarInventoryList.MaxWeaponSlots)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void UWarInventoryComponent::AddWeaponToSlot(UWarInventoryItemInstance* Weapon)
{
	WarInventoryList.WarWeaponSlots.AddUnique(Weapon);
}

void UWarInventoryComponent::ReplaceActiveWeapon(UWarInventoryItemInstance* Weapon)
{
	WarInventoryList.RemoveEntry(WarInventoryList.WarWeaponSlots[ActiveWeaponSlotIndex]);
	UnequipWeaponInSlot(ActiveWeaponSlotIndex);
	WarInventoryList.WarWeaponSlots[ActiveWeaponSlotIndex]->MarkAsGarbage();
	WarInventoryList.AddEntry(Weapon);
	EquipWeaponInSlot(ActiveWeaponSlotIndex);
	WarInventoryList.WarWeaponSlots[ActiveWeaponSlotIndex] = Weapon;
}

void UWarInventoryComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if (WarInventoryList.WarWeaponSlots.IsValidIndex(NewIndex) && (ActiveWeaponSlotIndex != NewIndex))
	{
		UnequipWeaponInSlot(ActiveWeaponSlotIndex);
		ActiveWeaponSlotIndex = NewIndex;
		EquipWeaponInSlot(ActiveWeaponSlotIndex);
	}
}
