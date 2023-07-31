// Person Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WarCharacterMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct FWarCharacterGroundInformation
{
	GENERATED_BODY()
	FWarCharacterGroundInformation(): LastUpdateFrame(0), GroundDistance(0.0f)
	{

	}
	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API UWarCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UWarCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "War|CharacterMovementComponent", meta = (BlueprintThreadSafe))
	const FWarCharacterGroundInformation& GetCharacterGroundInformation();

protected:
	FWarCharacterGroundInformation WarCharacterGroundInformationCache;

private:
	const float GroundTraceDistance = 100000.0f;
};
