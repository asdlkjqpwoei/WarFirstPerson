// Personal Copyright
#include "Character/WarHealthComponent.h"

// Sets default values for this component's properties
UWarHealthComponent::UWarHealthComponent() : CurrentHealth(0), MaxHealth(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UWarHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWarHealthComponent::SetCurrentHealth(int32 NewCurrentHealth)
{
	CurrentHealth = NewCurrentHealth;
	if (CurrentHealth <= 0)
	{
		OnCurrentHealthLessThanOne();
	}
}

int32 UWarHealthComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

void UWarHealthComponent::SetMaxHealth(int32 NewMaxHealth)
{
	if (NewMaxHealth > 0)
	{
		MaxHealth = NewMaxHealth;
	}
}

int32 UWarHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}

// Called when the game starts
void UWarHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}
