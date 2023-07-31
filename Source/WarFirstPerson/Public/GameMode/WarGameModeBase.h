// Personal Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WarGameModeBase.generated.h"

class UWarPawnData;

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API AWarGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AWarGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	UFUNCTION(BlueprintCallable, Category = "War|Pawn")
	const UWarPawnData* GetWarPawnDataForController(const AController* InController) const;
};
