// Personal Copyright
#include "Player/WarPlayerState.h"
#include "Player/WarPlayerController.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(WarPlayerState)

AWarPlayerState::AWarPlayerState(const FObjectInitializer& ObjectInitializer)
{

}

int32 AWarPlayerState::GetTeamID() const
{
	return GenericTeamIdToInteger(TeamID);
}

void AWarPlayerState::SetGenericTeamID(const FGenericTeamId& NewGenericTeamID)
{
	TeamID = NewGenericTeamID;
}

AWarPlayerController* AWarPlayerState::GetWarPlayerController() const
{
	return Cast<AWarPlayerController>(GetOwner());
}

template <class T>
const T* AWarPlayerState::GetWarPawnData() const
{
	return Cast<T>(WarPawnData);
}
