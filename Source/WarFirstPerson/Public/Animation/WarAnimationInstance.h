// Personal Copyright

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WarAnimationInstance.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class WARFIRSTPERSON_API UWarAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UWarAnimationInstance(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
