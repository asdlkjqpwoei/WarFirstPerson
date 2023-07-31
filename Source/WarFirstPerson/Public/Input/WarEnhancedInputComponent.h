// Personal Copyright

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "WarEnhancedInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UWarInputConfig;

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API UWarEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	UWarEnhancedInputComponent(const FObjectInitializer& ObjectInitializer);
	void AddInputMappings(const UWarInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem) const;
};
