// Personal Copyright
#include "Player/WarPlayerController.h"
#include "Player/WarPlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WarPlayerController)

AWarPlayerController::AWarPlayerController(const FObjectInitializer& ObjectInitializer)
{
}

AWarPlayerState* AWarPlayerController::GetWarPlayerState() const
{
	return CastChecked<AWarPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}
