// Personal Copyright
#pragma once
#include "GameFramework/GameStateBase.h"
#include "WarGameStateBase.generated.h"

class UWarTeamGameStateComponent;

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API AWarGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AWarGameStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "War|Game State")
	TObjectPtr<UWarTeamGameStateComponent> WarTeamGameStateComponent;
};
