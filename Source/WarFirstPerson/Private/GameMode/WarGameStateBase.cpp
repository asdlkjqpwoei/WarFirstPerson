// Personal Copyright
#include "GameMode/WarGameStateBase.h"
#include "Team/WarTeamGameStateComponent.h"

AWarGameStateBase::AWarGameStateBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WarTeamGameStateComponent = CreateDefaultSubobject<UWarTeamGameStateComponent>(TEXT("WarTeamGameStateComponent"));
}
