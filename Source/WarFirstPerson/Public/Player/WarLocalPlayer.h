// Personal Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "WarLocalPlayer.generated.h"

class UInputMappingContext;
class UWarGameUserSettings;

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API UWarLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
public:
	UWarLocalPlayer();

	UFUNCTION()
	UWarGameUserSettings* GetLocalSettings() const;

private:
	UPROPERTY(Transient)
	mutable TObjectPtr<const UInputMappingContext> InputMappingContext;
};
