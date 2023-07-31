// Personal Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "WarTeamGameStateComponent.generated.h"

class UWarTeamDefinition;

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API UWarTeamGameStateComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	UWarTeamGameStateComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	void RegisterTeamsWithTeamManagerSubsystem() const;
	void UnregisterTeamsWithTeamManagerSubsystem() const;
	void AssignPlayerToTeam();
	int32 GetLeastPopulatedTeamID() const;

private:
	TMap<uint8, TObjectPtr<UWarTeamDefinition>> Teams;
};
