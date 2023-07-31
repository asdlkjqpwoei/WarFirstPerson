// Personal Copyright
#include "Team/WarTeamGameStateComponent.h"
#include "GameMode/WarGameModeBase.h"
#include "Player/WarPlayerState.h"
#include "Team/WarTeamDefinition.h"
#include "Team/WarTeamManagerSubsystem.h"

UWarTeamGameStateComponent::UWarTeamGameStateComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWarTeamGameStateComponent::BeginPlay()
{
	Super::BeginPlay();
	RegisterTeamsWithTeamManagerSubsystem();
	AssignPlayerToTeam();
}

void UWarTeamGameStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnregisterTeamsWithTeamManagerSubsystem();
}

void UWarTeamGameStateComponent::AssignPlayerToTeam()
{
	const AGameStateBase* GameStateBase = GetGameStateChecked<AGameStateBase>();
	for (APlayerState* PlayerState : GameStateBase->PlayerArray)
	{
		if (AWarPlayerState* WarPlayerState = Cast<AWarPlayerState>(PlayerState))
		{
			if (WarPlayerState->IsOnlyASpectator())
			{
				WarPlayerState->SetGenericTeamId(FGenericTeamId::NoTeam);
			}
			else
			{
				const FGenericTeamId GenericTeamID = WarPlayerState->IntegerToGenericTeamId(GetLeastPopulatedTeamID());
				WarPlayerState->SetGenericTeamID(GenericTeamID);
			}
		}
	}
}

int32 UWarTeamGameStateComponent::GetLeastPopulatedTeamID() const
{
	const int32 NumberOfTeams = Teams.Num();
	if (NumberOfTeams > 0)
	{
		TMap<int32, uint32> TeamMembers;
		TeamMembers.Reserve(NumberOfTeams);
		for (const TTuple<uint8, TObjectPtr<UWarTeamDefinition>> Team : Teams)
		{
			const int32 TeamID = Team.Key;
			TeamMembers.Add(TeamID, 0);
		}
		const AGameStateBase* GameStateBase = GetGameStateChecked<AGameStateBase>();
		for (const APlayerState* PlayerState : GameStateBase->PlayerArray)
		{
			if (const AWarPlayerState* WarPlayerState = Cast<AWarPlayerState>(PlayerState))
			{
				const int32 PlayerTeamID = WarPlayerState->GetTeamID();
				if ((PlayerTeamID != INDEX_NONE) && !WarPlayerState->IsInactive())
				{
					check(TeamMembers.Contains(PlayerTeamID));
					TeamMembers[PlayerTeamID]++;
				}
			}
		}
		int32 LeastPopulatedTeamID = INDEX_NONE;
		uint32 LeastPopulatedPlayerCount = TNumericLimits<uint32>::Max();
		for (const TTuple<int32, uint32> TeamMember : TeamMembers)
		{
			const int32 TemporaryTeamID = TeamMember.Key;
			const uint32 TemporaryTeamPlayerCount = TeamMember.Value;
			if (TemporaryTeamPlayerCount < LeastPopulatedPlayerCount)
			{
				LeastPopulatedTeamID = TemporaryTeamID;
				LeastPopulatedPlayerCount = TemporaryTeamPlayerCount;
			}
			else if (TemporaryTeamPlayerCount == LeastPopulatedPlayerCount)
			{
				if ((TemporaryTeamID < LeastPopulatedTeamID) || (LeastPopulatedTeamID == INDEX_NONE))
				{
					LeastPopulatedTeamID = TemporaryTeamID;
					LeastPopulatedPlayerCount = TemporaryTeamPlayerCount;
				}
			}
		}
		return LeastPopulatedTeamID;
	}
	return INDEX_NONE;
}

void UWarTeamGameStateComponent::RegisterTeamsWithTeamManagerSubsystem() const
{
	for (const TTuple<uint8, TObjectPtr<UWarTeamDefinition>> Team : Teams)
	{
		if (Team.Value->GetTeamID() != INDEX_NONE)
		{
			UWarTeamManagerSubsystem* WarTeamManagerSubsystem = GetWorld()->GetSubsystem<UWarTeamManagerSubsystem>();
			if (ensure(WarTeamManagerSubsystem))
			{
				WarTeamManagerSubsystem->RegisterTeam(Team.Value);
			}
		}
	}
}

void UWarTeamGameStateComponent::UnregisterTeamsWithTeamManagerSubsystem() const
{
	for (const TTuple<uint8, TObjectPtr<UWarTeamDefinition>> Team : Teams)
	{
		if (Team.Value->GetTeamID() != INDEX_NONE)
		{
			UWarTeamManagerSubsystem* WarTeamManagerSubsystem = GetWorld()->GetSubsystem<UWarTeamManagerSubsystem>();
			if (ensure(WarTeamManagerSubsystem))
			{
				WarTeamManagerSubsystem->UnregisterTeam(Team.Value);
			}
		}
	}
}
