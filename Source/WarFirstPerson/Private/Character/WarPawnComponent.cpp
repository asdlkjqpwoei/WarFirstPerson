// Personal Copyright
#include "Character/WarPawnComponent.h"
#include "Character/WarPawnData.h"

UWarPawnComponent::UWarPawnComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WarPawnData = nullptr;
}

void UWarPawnComponent::SetPawnData(const UWarPawnData* InPawnData)
{
	check(InPawnData)
	WarPawnData = InPawnData;
}