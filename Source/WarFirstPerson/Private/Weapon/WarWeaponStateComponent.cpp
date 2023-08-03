// Personal Copyright
#include "Weapon/WarWeaponStateComponent.h"

// Sets default values for this component's properties
UWarWeaponStateComponent::UWarWeaponStateComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UWarWeaponStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWarWeaponStateComponent::SetCurrentWarWeaponState(const EWarWeaponState& NewWarWeaponState)
{
	CurrentWarWeaponState = NewWarWeaponState;
}

EWarWeaponState UWarWeaponStateComponent::GetCurrentWarWeaponState() const
{
	return CurrentWarWeaponState;
}

// Called when the game starts
void UWarWeaponStateComponent::BeginPlay()
{
	Super::BeginPlay();	
}
