// Personal Copyright

#pragma once

#include "CoreMinimal.h"
#include "WarPawnComponent.generated.h"

class UWarPawnData;
/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API UWarPawnComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UWarPawnComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "War|Pawn")
	static UWarPawnComponent* FindPawnComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UWarPawnComponent>() : nullptr); }

	template <class T>
	const T* GetPawnData() const { return Cast<T>(WarPawnData); }

	void SetPawnData(const UWarPawnData* InPawnData);

protected:

	UPROPERTY(EditInstanceOnly, Category = "War|Pawn")
	TObjectPtr<const UWarPawnData> WarPawnData;
};
