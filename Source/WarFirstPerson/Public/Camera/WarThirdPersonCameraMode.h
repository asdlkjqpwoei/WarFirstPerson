// Personal Copyright
#pragma once
#include "Camera/WarCameraMode.h"
#include "WarThirdPersonCameraMode.generated.h"

class UCurveVector;

USTRUCT(BlueprintType)
struct FWarPenetrationAvoidanceFeeler
{
	GENERATED_BODY()

	FWarPenetrationAvoidanceFeeler() : AdjustmentRotator(ForceInit), WorldWeight(0), PawnWeight(0), Extent(0), TraceInterval(0), FramesUntilNextTrace(0)
	{

	}

	FWarPenetrationAvoidanceFeeler(const FRotator& NewAdjustMentRotator, const float& NewWorldWeight, const float& NewPawnWeight, const float& NewExtent, const int32& NewTraceInterval = 0, const int32& NewFramesUntilNextTrace = 0) : AdjustmentRotator(NewAdjustMentRotator), WorldWeight(NewWorldWeight), PawnWeight(NewPawnWeight), Extent(NewExtent), TraceInterval(NewTraceInterval), FramesUntilNextTrace(NewFramesUntilNextTrace)
	{

	}

	UPROPERTY(EditDefaultsOnly, Category = "War|Camera Penetration Avoidance Feeler")
	FRotator AdjustmentRotator;

	UPROPERTY(EditDefaultsOnly, Category = "War|Camera Penetration Avoidance Feeler")
	float WorldWeight;

	UPROPERTY(EditDefaultsOnly, Category = "War|Camera Penetration Avoidance Feeler")
	float PawnWeight;

	UPROPERTY(EditDefaultsOnly, Category = "War|Camera Penetration Avoidance Feeler")
	float Extent;

	UPROPERTY(EditDefaultsOnly, Category = "War|Camera Penetration Avoidance Feeler")
	int32 TraceInterval;

	UPROPERTY(Transient)
	int32 FramesUntilNextTrace;
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class WARFIRSTPERSON_API UWarThirdPersonCameraMode : public UWarCameraMode
{
	GENERATED_BODY()
public:
	UWarThirdPersonCameraMode();
protected:
	void SetFocusCrouchOffset(FVector NewFocusCrouchOffset);
	virtual void UpdateCameraModeData(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "War|Third Person Camera", Meta = (EditCondition = "!bUseRuntimeFloatCurves"))
	TObjectPtr<const UCurveVector> OwnerActorOffsetCurve;

	UPROPERTY(EditDefaultsOnly, Category = "War|Third Person Camera")
	bool bUseRuntimeFloatCurves;

	UPROPERTY(EditDefaultsOnly, Category = "War|Third Person Camera", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
	FRuntimeFloatCurve OwnerActorOffsetX;

	UPROPERTY(EditDefaultsOnly, Category = "War|Third Person Camera", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
	FRuntimeFloatCurve OwnerActorOffsetY;

	UPROPERTY(EditDefaultsOnly, Category = "War|Third Person Camera", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
	FRuntimeFloatCurve OwnerActorOffsetZ;

	UPROPERTY(EditDefaultsOnly, Category = "War|Third Person Camera")
	float CrouchOffsetBlendMultiplier = 5.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Camera Collision")
	float PenetrationBlendInTime = 0.1f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Camera Collision")
	float PenetrationBlendOutTime = 0.15f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Camera Collision")
	bool bPreventPenetration = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Camera Collision")
	bool bDoPredictiveAvoidance = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Camera Collision")
	float CollisionPushOutDistance = 2.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Camera Collision")
	float ReportPenetrationPercent = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Camera Collision")
	TArray<FWarPenetrationAvoidanceFeeler> PenetrationAvoidanceFeelers;

	UPROPERTY(Transient)
	float AimLineToDesiredPosBlockedPercent;

	FVector InitialCrouchOffset = FVector::ZeroVector;
	FVector OwnerCrouchOffset = FVector::ZeroVector;
	float CrouchOffsetBlendPercent = 1.0f;
	FVector CurrentCrouchOffset = FVector::ZeroVector;
};
