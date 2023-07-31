// Personal Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WarPlayerController.generated.h"

class AWarPlayerState;

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API AWarPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AWarPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "War|PlayerController")
	AWarPlayerState* GetWarPlayerState() const;
};
