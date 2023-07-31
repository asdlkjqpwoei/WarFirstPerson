// Personal Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WarInventoryComponent.generated.h"

class UWarInventoryItemInstance;
class UWarInventoryItemDefinition;

USTRUCT(BlueprintType)
struct FWarInventoryEntry
{
	GENERATED_BODY()
	FWarInventoryEntry()
	{

	}
	FString GetDebugString() const;
private:
	friend FWarInventoryList;
	friend UWarInventoryComponent;

	UPROPERTY()
	TObjectPtr<UWarInventoryItemInstance> ItemInstance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;
};

USTRUCT(BlueprintType)
struct FWarInventoryList
{
	GENERATED_BODY()
	FWarInventoryList() : OwnerComponent(nullptr)
	{

	}
	FWarInventoryList(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent)
	{

	}

public:
	TArray<UWarInventoryItemInstance*> GetAllItems() const;
	void AddEntry(UWarInventoryItemInstance* WarItemInstance, int32 StackCount = 1);
	void RemoveEntry(UWarInventoryItemInstance* WarInventoryItemInstance);
private:
	friend UWarInventoryComponent;

	UPROPERTY()
	int32 MaxWeaponSlots = 4;

	UPROPERTY()
	TArray<FWarInventoryEntry> WarInventoryEntries;

	UPROPERTY()
	TArray<TObjectPtr<UWarInventoryItemInstance>> WarWeaponSlots;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARFIRSTPERSON_API UWarInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWarInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category = "War|Inventory", BlueprintPure)
	UWarInventoryItemInstance* FindItemByDefinition(TSubclassOf<UWarInventoryItemDefinition> ItemDefinition) const;

	UFUNCTION(BlueprintCallable, Category = "War|Inventory")
	void AddItemInstance(UWarInventoryItemInstance* WarItemInstance, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, Category = "War|Inventory")
	void RemoveItemInstance(UWarInventoryItemInstance* WarItemInstance);

	UFUNCTION(BlueprintCallable, Category = "War|Inventory")
	TArray<UWarInventoryItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category = "War|Inventory")
	void CycleActiveWeaponForward();

	UFUNCTION(BlueprintCallable, Category = "War|Inventory")
	void CycleActiveWeaponBackward();

	UFUNCTION(BlueprintCallable, Category = "War|Inventory")
	TArray<UWarInventoryItemInstance*> GetSlots() const;

	UFUNCTION(BlueprintCallable, Category ="War|Inventory")
	bool IsWeaponSlotsFull() const;

	UFUNCTION(BlueprintCallable, Category = "War|Inventory")
	void AddWeaponToSlot(UWarInventoryItemInstance* Weapon);

	UFUNCTION(BlueprintCallable, Category = "War|Inventory")
	void ReplaceActiveWeapon(UWarInventoryItemInstance* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "War|Inventory")
	void SetActiveSlotIndex(int32 NewIndex);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void UnequipWeaponInSlot(int32 WeaponIndex);
	void EquipWeaponInSlot(int32 WeaponIndex);

private:
	UPROPERTY()
	TObjectPtr<UWarInventoryItemInstance> ActiveWeapon;

	UPROPERTY()
	int32 ActiveWeaponSlotIndex = -1;

	UPROPERTY()
	FWarInventoryList WarInventoryList;
};
