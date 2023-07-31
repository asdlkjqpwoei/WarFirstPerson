// Personal Copyright
#pragma once
#include "Subsystems/WorldSubsystem.h"
#include "WarTeamManagerSubsystem.generated.h"

class AWarPlayerState;
class UWarTeamDefinition;

USTRUCT(BlueprintType)
struct FWarTeam
{
	GENERATED_BODY()
public:
	void SetTeamDefinition(UWarTeamDefinition* NewWarTeamDefinition);
	void RemoveTeamDefinition(const UWarTeamDefinition* WarTeamDefinitionToRemove);

	UPROPERTY(VisibleAnywhere, Category = "War|Team")
	TObjectPtr<UWarTeamDefinition> WarTeamDefinition;
};

UENUM(BlueprintType)
enum class EWarTeamComparionResult : uint8
{
	OnSameTeam,
	DifferentTeams,
	InvalidArgument
};

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API UWarTeamManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	bool RegisterTeam(UWarTeamDefinition* WarTeamDefinition);
	bool UnregisterTeam(UWarTeamDefinition* WarTeamDefinition);
	bool CanDealDamage(const AActor* DamageDealer, const AActor* DamageReceiver, bool bAllowDamageToSelf = true) const;
	bool ChangeTeamForActor(const AActor* Actor, int32 NewTeamID);
	const AWarPlayerState* FindPlayerStateFromActor(const AActor* Actor) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "War|Team", Meta = (Keywords = "Get"))
	int32 FindTeamFromActor(const AActor* Actor) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "War|Team", Meta = (ExpandEnumAsExecs = ReturnValue))
	EWarTeamComparionResult CompareTeam(const AActor* ActorA, const AActor* ActorB) const;

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "War|Team", Meta = (AllowPrivateAccess = "true"))
	TMap<int32, FWarTeam> Teams;
};
