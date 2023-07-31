// Personal Copyright
#include "Team/WarTeamManagerSubsystem.h"
#include "Player/WarPlayerState.h"
#include "Team/WarTeamAgentInterface.h"
#include "Team/WarTeamDefinition.h"
#include "WarFirstPerson/WarLogChannels.h"

bool UWarTeamManagerSubsystem::RegisterTeam(UWarTeamDefinition* WarTeamDefinition)
{
	if (!ensure(WarTeamDefinition))
	{
		return false;
	}
	const int32 TeamID = WarTeamDefinition->GetTeamID();
	if (ensure(TeamID != INDEX_NONE))
	{
		FWarTeam& WarTeam = Teams.FindOrAdd(TeamID);
		WarTeam.SetTeamDefinition(WarTeamDefinition);
		return true;
	}
	return false;
}

bool UWarTeamManagerSubsystem::UnregisterTeam(UWarTeamDefinition* WarTeamDefinition)
{
	if (!ensure(WarTeamDefinition))
	{
		return false;
	}
	const int32 TeamID = WarTeamDefinition->GetTeamID();
	if (ensure(TeamID != INDEX_NONE))
	{
		FWarTeam* WarTeam = Teams.Find(TeamID);
		if (WarTeam)
		{
			WarTeam->RemoveTeamDefinition(WarTeamDefinition);
			return true;
		}
	}
	return false;
}

bool UWarTeamManagerSubsystem::CanDealDamage(const AActor* DamageDealer, const AActor* DamageReceiver, bool bAllowDamageToSelf) const
{
	if (bAllowDamageToSelf)
	{
		if ((DamageDealer == DamageReceiver) || (FindPlayerStateFromActor(DamageDealer) == FindPlayerStateFromActor(DamageReceiver)))
		{
			return true;
		}
	}
	const EWarTeamComparionResult WarTeamComparionResult = CompareTeam(DamageDealer, DamageReceiver);
	if (WarTeamComparionResult == EWarTeamComparionResult::DifferentTeams)
	{
		return true;
	}
	return false;
}

bool UWarTeamManagerSubsystem::ChangeTeamForActor(const AActor* Actor, int32 NewTeamID)
{
	if (AWarPlayerState* WarPlayerState = const_cast<AWarPlayerState*>(FindPlayerStateFromActor(Actor)))
	{
		const FGenericTeamId NewGenericTeamID = WarPlayerState->IntegerToGenericTeamId(NewTeamID);
		WarPlayerState->SetGenericTeamID(NewGenericTeamID);
		return true;
	}
	else
	{
		return false;
	}
}

const AWarPlayerState* UWarTeamManagerSubsystem::FindPlayerStateFromActor(const AActor* Actor) const
{
	if (Actor)
	{
		if (const APawn* Pawn = Cast<const APawn>(Actor))
		{
			if (const AWarPlayerState* WarPlayerState = Pawn->GetPlayerState<AWarPlayerState>())
			{
				return WarPlayerState;
			}
		}
		else if (const AController* Controller = Cast<const AController>(Actor))
		{
			if (const AWarPlayerState* WarPlayerState = Cast<AWarPlayerState>(Controller->PlayerState))
			{
				return WarPlayerState;
			}
		}
		else if (const AWarPlayerState* WarPlayerState = Cast<const AWarPlayerState>(Actor))
		{
			return WarPlayerState;
		}
	}
	return nullptr;
}

int32 UWarTeamManagerSubsystem::FindTeamFromActor(const AActor* Actor) const
{
	if (Actor)
	{
		if (const IWarTeamAgentInterface* WarTeamAgentInteface = Cast<IWarTeamAgentInterface>(Actor->GetInstigator()))
		{
			return WarTeamAgentInteface->GenericTeamIdToInteger(WarTeamAgentInteface->GetGenericTeamId());
		}
		if (const AWarPlayerState* WarPlayerState = FindPlayerStateFromActor(Actor))
		{
			return WarPlayerState->GetTeamID();
		}
	}
	return INDEX_NONE;
}

EWarTeamComparionResult UWarTeamManagerSubsystem::CompareTeam(const AActor* ActorA, const AActor* ActorB) const
{
	const int32& TeamIDA = FindTeamFromActor(ActorA);
	const int32& TeamIDB = FindTeamFromActor(ActorB);
	if ((TeamIDA == INDEX_NONE) || (TeamIDB == INDEX_NONE))
	{
		return EWarTeamComparionResult::InvalidArgument;
	}
	else
	{
		return (TeamIDA == TeamIDB) ? EWarTeamComparionResult::OnSameTeam : EWarTeamComparionResult::DifferentTeams;
	}
}

void FWarTeam::SetTeamDefinition(UWarTeamDefinition* NewWarTeamDefinition)
{
	ensure(NewWarTeamDefinition);
	const UWarTeamDefinition* OldWarTeamDefinition = WarTeamDefinition;
	if (OldWarTeamDefinition != NewWarTeamDefinition)
	{
		WarTeamDefinition = NewWarTeamDefinition;
	}
	else
	{
		UE_LOG(LogWarTeam, Warning, TEXT("New team definition is same as the old, team change operation denied."));
	}
}

void FWarTeam::RemoveTeamDefinition(const UWarTeamDefinition* WarTeamDefinitionToRemove)
{
	if (WarTeamDefinition == WarTeamDefinitionToRemove)
	{
		WarTeamDefinition == nullptr;
	}
	else
	{
		UE_LOG(LogWarTeam, Warning, TEXT("No war team definition to remove."));
	}
}
