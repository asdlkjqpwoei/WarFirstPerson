// Personal Copyright
#include "Weapon/WarWeaponAmmunitionComponent.h"

// Sets default values for this component's properties
UWarWeaponAmmunitionComponent::UWarWeaponAmmunitionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called every frame
void UWarWeaponAmmunitionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWarWeaponAmmunitionComponent::SetCurrentMagazineCapacity(const int32& NewCurrentMagazineCapacity)
{
	CurrentMagazineCapacity = NewCurrentMagazineCapacity;
}

int32 UWarWeaponAmmunitionComponent::GetCurrentMagazineCapacity() const
{
	return CurrentMagazineCapacity;
}

void UWarWeaponAmmunitionComponent::SetCurrentAmmunitionCapacity(const int32& NewCurrentAmmunitionCapacity)
{
	CurrentAmmunitionCapacity = NewCurrentAmmunitionCapacity;
}

int32 UWarWeaponAmmunitionComponent::GetCurrentAmmunitionCapacity() const
{
	return CurrentAmmunitionCapacity;
}

void UWarWeaponAmmunitionComponent::SetMaxMagazineCapacity(const int32& NewMaxMagazineCapacity)
{
	MaxMagazineCapacity = NewMaxMagazineCapacity;
}

int32 UWarWeaponAmmunitionComponent::GetMaxMagazineCapacity() const
{
	return MaxMagazineCapacity;
}

void UWarWeaponAmmunitionComponent::SetMaxAmmunitionCapacity(const int32& NewMaxAmmunitionCapacity)
{
	MaxAmmunitionCapacity = NewMaxAmmunitionCapacity;
}

int32 UWarWeaponAmmunitionComponent::GetMaxAmmunitionCapacity() const
{
	return MaxAmmunitionCapacity;
}

// Called when the game starts
void UWarWeaponAmmunitionComponent::BeginPlay()
{
	Super::BeginPlay();
}
