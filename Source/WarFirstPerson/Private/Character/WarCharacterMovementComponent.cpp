// Person Copyright
#include "Character/WarCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WarCharacterMovementComponent)

UWarCharacterMovementComponent::UWarCharacterMovementComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}

const FWarCharacterGroundInformation& UWarCharacterMovementComponent::GetCharacterGroundInformation()
{
	if (!CharacterOwner || (GFrameCounter == WarCharacterGroundInformationCache.LastUpdateFrame))
	{
		return WarCharacterGroundInformationCache;
	}
	if (MovementMode == MOVE_Walking)
	{
		WarCharacterGroundInformationCache.GroundHitResult = CurrentFloor.HitResult;
	}
	else
	{
		const UCapsuleComponent* CapsuleComponent = CharacterOwner->GetCapsuleComponent();
		check(CapsuleComponent);
		const float CapsuleHalfHeight = CapsuleComponent->GetUnscaledCapsuleHalfHeight();
		const ECollisionChannel CollisionChannel = UpdatedComponent ? UpdatedComponent->GetCollisionObjectType(): ECC_Pawn;
		const FVector TraceStart(GetActorLocation());
		const FVector TraceEnd(TraceStart.X, TraceStart.Y, (TraceStart.Z - GroundTraceDistance - CapsuleHalfHeight));
		FCollisionQueryParams QueryParameters(SCENE_QUERY_STAT(WarCharacterMovementComponent_GetCharacterGroundInformation), false, CharacterOwner);
		FCollisionResponseParams ResponseParameters;
		InitCollisionParams(QueryParameters, ResponseParameters);
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParameters, ResponseParameters);
		WarCharacterGroundInformationCache.GroundHitResult = HitResult;
		WarCharacterGroundInformationCache.GroundDistance = GroundTraceDistance;
		if (MovementMode == MOVE_NavWalking)
		{
			WarCharacterGroundInformationCache.GroundDistance = 0.0f;
		}
		else if (HitResult.bBlockingHit)
		{
			WarCharacterGroundInformationCache.GroundDistance = FMath::Max((HitResult.Distance - CapsuleHalfHeight), 0.0f);
		}
	}
	WarCharacterGroundInformationCache.LastUpdateFrame = GFrameCounter;
	return WarCharacterGroundInformationCache;
}