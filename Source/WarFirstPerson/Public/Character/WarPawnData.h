// Personal Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Templates/SubClassOf.h"
#include "WarPawnData.generated.h"

class UWarCameraMode;
class UWarInputConfig;

/**
 * 
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "War Pawn Data" , ShortTooltip = "Data asset used to define a Pawn."))
class WARFIRSTPERSON_API UWarPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UWarPawnData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "War|Input")
	TObjectPtr<UWarInputConfig> WarInputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "War|Camera")
	TSubclassOf<UWarCameraMode> DefaultCameraMode;
};
