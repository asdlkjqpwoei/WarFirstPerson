// Personal Copyright
#pragma once
#include "GameFramework/PlayerState.h"
#include "Team/WarTeamAgentInterface.h"
#include "WarPlayerState.generated.h"

class AWarPlayerController;
class UWarPawnData;

/**
 * 
 */
UCLASS(Config = Game)
class WARFIRSTPERSON_API AWarPlayerState : public APlayerState, public IWarTeamAgentInterface
{
	GENERATED_BODY()
public:
	AWarPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "War|Player State")
	AWarPlayerController* GetWarPlayerController() const;

	void SetGenericTeamID(const FGenericTeamId& NewGenericTeamID);

	UFUNCTION(BlueprintCallable, Category = "War|Player State")
	int32 GetTeamID() const;

	template <class T>
	const T* GetWarPawnData() const;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "War|Player State")
	TObjectPtr<const UWarPawnData> WarPawnData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "War|Player State")
	FGenericTeamId TeamID;
};
