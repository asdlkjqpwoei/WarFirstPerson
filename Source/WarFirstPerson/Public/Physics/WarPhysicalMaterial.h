// Personal Copyright

#pragma once

#include "GameplayTagContainer.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WarPhysicalMaterial.generated.h"

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API UWarPhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()
public:
	UWarPhysicalMaterial(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PhysicalProperties)
	FGameplayTagContainer GameplayTagContainer;
};
