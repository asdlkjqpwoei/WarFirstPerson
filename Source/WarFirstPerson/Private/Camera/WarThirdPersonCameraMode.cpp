// Personal Copyright
#include "Camera/WarThirdPersonCameraMode.h"
#include "Camera/WarCameraComponent.h"
#include "Character/WarCharacter.h"
#include "Curves/CurveVector.h"
#include "GameFramework/CameraBlockingVolume.h"

UWarThirdPersonCameraMode::UWarThirdPersonCameraMode()
{
	OwnerActorOffsetCurve = nullptr;
	PenetrationAvoidanceFeelers.Add(FWarPenetrationAvoidanceFeeler(FRotator(+00.0f, +00.0f, 0.0f), 1.00f, 1.00f, 14.0f, 0));
	PenetrationAvoidanceFeelers.Add(FWarPenetrationAvoidanceFeeler(FRotator(+00.0f, +16.0f, 0.0f), 0.75f, 0.75f, 00.0f, 3));
	PenetrationAvoidanceFeelers.Add(FWarPenetrationAvoidanceFeeler(FRotator(+00.0f, -16.0f, 0.0f), 0.75f, 0.75f, 00.0f, 3));
	PenetrationAvoidanceFeelers.Add(FWarPenetrationAvoidanceFeeler(FRotator(+00.0f, +32.0f, 0.0f), 0.50f, 0.50f, 00.0f, 5));
	PenetrationAvoidanceFeelers.Add(FWarPenetrationAvoidanceFeeler(FRotator(+00.0f, -32.0f, 0.0f), 0.50f, 0.50f, 00.0f, 5));
	PenetrationAvoidanceFeelers.Add(FWarPenetrationAvoidanceFeeler(FRotator(+20.0f, +00.0f, 0.0f), 1.00f, 1.00f, 00.0f, 4));
	PenetrationAvoidanceFeelers.Add(FWarPenetrationAvoidanceFeeler(FRotator(-20.0f, +00.0f, 0.0f), 0.50f, 0.50f, 00.0f, 4));
}

void UWarThirdPersonCameraMode::SetFocusCrouchOffset(FVector NewFocusOffset)
{
	CrouchOffsetBlendPercent = 0.0f;
	InitialCrouchOffset = CurrentCrouchOffset;
	OwnerCrouchOffset = NewFocusOffset;
}

void UWarThirdPersonCameraMode::UpdateCameraModeData(float DeltaTime)
{
	if (const AWarCharacter* OwnerWarChracter = Cast<AWarCharacter>(GetOwnerActor()))
	{
		if (OwnerWarChracter->bIsCrouched)
		{
			const AWarCharacter* FocusWarCharacterClassDefaultObject = OwnerWarChracter->GetClass()->GetDefaultObject<AWarCharacter>();
			const float CrouchedHeightAdjustment = FocusWarCharacterClassDefaultObject->CrouchedEyeHeight - FocusWarCharacterClassDefaultObject->BaseEyeHeight;
			SetFocusCrouchOffset(FVector(0.0f, 0.0f, CrouchedHeightAdjustment));
		}
	}
	else
	{
		SetFocusCrouchOffset(FVector::ZeroVector);
	}
	if (CrouchOffsetBlendPercent < 1.0f)
	{
		CrouchOffsetBlendPercent = FMath::Min(CrouchOffsetBlendPercent + DeltaTime * CrouchOffsetBlendMultiplier, 1.0f);
		CurrentCrouchOffset = FMath::InterpEaseInOut(InitialCrouchOffset, OwnerCrouchOffset, CrouchOffsetBlendPercent, 1.0f);
	}
	else
	{
		CurrentCrouchOffset = OwnerCrouchOffset;
		CrouchOffsetBlendPercent = 1.0f;
	}
	FVector OwnerActorPivotLocation = GetOwnerActorPivotLocation() + CurrentCrouchOffset;
	FRotator OwnerActorPivotRotation = GetOwnerActorPivotRotation();

	OwnerActorPivotRotation.Pitch = FMath::ClampAngle(OwnerActorPivotRotation.Pitch, ViewPitchMin, ViewPitchMax);
	WarCameraModeViewData.CurrentLocation = OwnerActorPivotLocation;
	WarCameraModeViewData.CurrentRotation = OwnerActorPivotRotation;
	WarCameraModeViewData.CurrentControlRotation = WarCameraModeViewData.CurrentRotation;
	WarCameraModeViewData.CurrentFieldOfView = FieldOfView;
	if (!bUseRuntimeFloatCurves)
	{
		if (OwnerActorOffsetCurve)
		{
			const FVector FocusActorOffset = OwnerActorOffsetCurve->GetVectorValue(OwnerActorPivotRotation.Pitch);
			WarCameraModeViewData.CurrentLocation = OwnerActorPivotLocation + OwnerActorPivotRotation.RotateVector(FocusActorOffset);
		}
	}
	else
	{
		FVector FocusActorOffset(0.0f);
		FocusActorOffset.X = OwnerActorOffsetX.GetRichCurveConst()->Eval(OwnerActorPivotRotation.Pitch);
		FocusActorOffset.Y = OwnerActorOffsetY.GetRichCurveConst()->Eval(OwnerActorPivotRotation.Pitch);
		FocusActorOffset.Z = OwnerActorOffsetZ.GetRichCurveConst()->Eval(OwnerActorPivotRotation.Pitch);
		WarCameraModeViewData.CurrentLocation = OwnerActorPivotLocation + OwnerActorPivotRotation.RotateVector(FocusActorOffset);
	}
	if (!bPreventPenetration)
	{
		return;
	}
	AActor* OwnerActor = GetOwnerActor();
	const UPrimitiveComponent* OwnerActorRootComponent = Cast<UPrimitiveComponent>(OwnerActor->GetRootComponent());
	if (OwnerActorRootComponent)
	{
		FVector ClosestPointOnLineToCapsuleCenter;
		FVector SafeLocation = OwnerActor->GetActorLocation();
		FMath::PointDistToLine(SafeLocation, WarCameraModeViewData.CurrentRotation.Vector(), WarCameraModeViewData.CurrentLocation, ClosestPointOnLineToCapsuleCenter);

		float const PushInDistance = PenetrationAvoidanceFeelers[0].Extent + CollisionPushOutDistance;
		float const MaxHalfHeight = OwnerActor->GetSimpleCollisionHalfHeight() - PushInDistance;
		SafeLocation.Z = FMath::Clamp(ClosestPointOnLineToCapsuleCenter.Z, SafeLocation.Z - MaxHalfHeight, SafeLocation.Z + MaxHalfHeight);

		float DistanceSquare;
		OwnerActorRootComponent->GetSquaredDistanceToCollision(ClosestPointOnLineToCapsuleCenter, DistanceSquare, SafeLocation);
		if (PenetrationAvoidanceFeelers.Num() > 0)
		{
			SafeLocation = SafeLocation + (SafeLocation - ClosestPointOnLineToCapsuleCenter).GetSafeNormal() * PushInDistance;
		}

		bool const bSingleRayPenetrationCheck = !bDoPredictiveAvoidance;
		float HardBlockedPercent = AimLineToDesiredPosBlockedPercent;
		float SoftBlockedPercent = AimLineToDesiredPosBlockedPercent;
		FVector BaseRay = WarCameraModeViewData.CurrentLocation - SafeLocation;
		FRotationMatrix BaseRayMatrix(BaseRay.Rotation());
		FVector BaseRayLocalUp, BaseRayLocalForward, BaseRayLocalRight;
		BaseRayMatrix.GetScaledAxes(BaseRayLocalForward, BaseRayLocalRight, BaseRayLocalUp);
		float DistanceBlockedPercentThisFrame = 1.0f;
		int32 const NumRaysToShoot = bSingleRayPenetrationCheck ? FMath::Min(1, PenetrationAvoidanceFeelers.Num()) : PenetrationAvoidanceFeelers.Num();
		FCollisionQueryParams SphereCollisionQueryParameters(SCENE_QUERY_STAT(CameraPen), false, nullptr);
		SphereCollisionQueryParameters.AddIgnoredActor(OwnerActor);
		FCollisionShape SphereShape = FCollisionShape::MakeSphere(0.0f);
		
		for (int32 RayIndex = 0; RayIndex < NumRaysToShoot; RayIndex++)
		{
			FWarPenetrationAvoidanceFeeler& WarPenetrationAvoidanceFeeler = PenetrationAvoidanceFeelers[RayIndex];
			if (WarPenetrationAvoidanceFeeler.FramesUntilNextTrace <= 0)
			{
				FVector RayTarget;
				{
					FVector RotatedRay = BaseRay.RotateAngleAxis(WarPenetrationAvoidanceFeeler.AdjustmentRotator.Yaw, BaseRayLocalUp);
					RotatedRay = RotatedRay.RotateAngleAxis(WarPenetrationAvoidanceFeeler.AdjustmentRotator.Pitch, BaseRayLocalRight);
					RayTarget = SafeLocation + RotatedRay;
				}
				SphereShape.Sphere.Radius = WarPenetrationAvoidanceFeeler.Extent;
				ECollisionChannel TraceChannel = ECC_Camera;
				FHitResult HitResult;
				const bool bHit = GetWorld()->SweepSingleByChannel(HitResult, SafeLocation, RayTarget, FQuat::Identity, TraceChannel, SphereShape, SphereCollisionQueryParameters);
				WarPenetrationAvoidanceFeeler.FramesUntilNextTrace = WarPenetrationAvoidanceFeeler.TraceInterval;
				const AActor* HitActor = HitResult.GetActor();
				if (bHit && HitActor)
				{
					bool bIgnoreHit = false;
					if (!bIgnoreHit && HitActor->IsA<ACameraBlockingVolume>())
					{
						const FVector OwnerViewForwardXY = OwnerActor->GetActorForwardVector().GetSafeNormal2D();
						const FVector OwnerViewLocation = OwnerActor->GetActorLocation();
						const FVector HitOffset = HitResult.Location - OwnerViewLocation;
						const FVector HitDirectionXY = HitOffset.GetSafeNormal2D();
						const float DotHitDirection = FVector::DotProduct(OwnerViewForwardXY, HitDirectionXY);
						if (DotHitDirection > 0.0f)
						{
							bIgnoreHit = true;
							SphereCollisionQueryParameters.AddIgnoredActor(HitActor);
						}
					}
					if (!bIgnoreHit)
					{
						float const Weight = Cast<APawn>(HitResult.GetActor()) ? WarPenetrationAvoidanceFeeler.PawnWeight : WarPenetrationAvoidanceFeeler.WorldWeight;
						float NewBlockPercent = HitResult.Time;
						NewBlockPercent = NewBlockPercent + (1.0f - NewBlockPercent) * (1.0f - Weight);
						NewBlockPercent = ((HitResult.Location - SafeLocation).Size() - CollisionPushOutDistance) / (RayTarget - SafeLocation).Size();
						DistanceBlockedPercentThisFrame = FMath::Min(NewBlockPercent, DistanceBlockedPercentThisFrame);
						WarPenetrationAvoidanceFeeler.FramesUntilNextTrace = 0;
					}
				}
				if (RayIndex == 0)
				{
					HardBlockedPercent = DistanceBlockedPercentThisFrame;
				}
				else
				{
					SoftBlockedPercent = DistanceBlockedPercentThisFrame;
				}
			}
			else
			{
				WarPenetrationAvoidanceFeeler.FramesUntilNextTrace--;
			}
		}
		if (bResetInterpolation)
		{
			AimLineToDesiredPosBlockedPercent = DistanceBlockedPercentThisFrame;
		}
		else if (AimLineToDesiredPosBlockedPercent < DistanceBlockedPercentThisFrame)
		{
			if (PenetrationBlendOutTime > DeltaTime)
			{
				AimLineToDesiredPosBlockedPercent = AimLineToDesiredPosBlockedPercent + DeltaTime / PenetrationBlendOutTime * (DistanceBlockedPercentThisFrame - AimLineToDesiredPosBlockedPercent);
			}
			else
			{
				AimLineToDesiredPosBlockedPercent = DistanceBlockedPercentThisFrame;
			}
		}
		else
		{
			if (AimLineToDesiredPosBlockedPercent > HardBlockedPercent)
			{
				AimLineToDesiredPosBlockedPercent = HardBlockedPercent;
			}
			else if (AimLineToDesiredPosBlockedPercent > SoftBlockedPercent)
			{
				if (PenetrationBlendInTime > DeltaTime)
				{
					AimLineToDesiredPosBlockedPercent = AimLineToDesiredPosBlockedPercent - DeltaTime / PenetrationBlendInTime * (AimLineToDesiredPosBlockedPercent - SoftBlockedPercent);
				}
				else
				{
					AimLineToDesiredPosBlockedPercent = SoftBlockedPercent;
				}
			}
		}
		AimLineToDesiredPosBlockedPercent = FMath::Clamp<float>(AimLineToDesiredPosBlockedPercent, 0.0f, 1.0f);
		if (AimLineToDesiredPosBlockedPercent < (1.0f - ZERO_ANIMWEIGHT_THRESH))
		{
			WarCameraModeViewData.CurrentLocation = SafeLocation + (WarCameraModeViewData.CurrentLocation - SafeLocation) * AimLineToDesiredPosBlockedPercent;
		}
	}
}
