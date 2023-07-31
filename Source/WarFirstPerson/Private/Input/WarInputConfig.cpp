// Personal Copyright
#include "Input/WarInputConfig.h"
#include "Containers/UnrealString.h"
#include "InputAction.h"
#include "WarFirstPerson/WarLogChannels.h"
#include "UObject/UObjectBaseUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WarInputConfig)

UWarInputConfig::UWarInputConfig(const FObjectInitializer& ObjectInitializer)
{

}

const UInputAction* UWarInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FWarInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogWarInput, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}