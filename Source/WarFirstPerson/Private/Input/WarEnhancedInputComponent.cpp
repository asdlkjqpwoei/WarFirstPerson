// Personal Copyright

#include "Input/WarEnhancedInputComponent.h"
#include "Input/WarInputConfig.h"
#include "Containers/Map.h"
#include "EnhancedInputSubsystems.h"
#include "Settings/WarGameUserSettings.h"

UWarEnhancedInputComponent::UWarEnhancedInputComponent(const FObjectInitializer& ObjectInitializer)
{

}

void UWarEnhancedInputComponent::AddInputMappings(const UWarInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem) const
{
	check(InputConfig)
	check(EnhancedInputLocalPlayerSubsystem);
	if (UWarGameUserSettings* LocalUserSettings = UWarGameUserSettings::Get())
	{
		for (const TPair<FName, FKey>& Pair : LocalUserSettings->GetCustomPlayerInputConfig())
		{
			if (Pair.Key != NAME_None && Pair.Value.IsValid())
			{
				EnhancedInputLocalPlayerSubsystem->AddPlayerMappedKeyInSlot(Pair.Key, Pair.Value);
			}
		}
	}
}
