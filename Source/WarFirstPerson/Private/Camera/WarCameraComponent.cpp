// Personal Copyright
#include "Camera/WarCameraComponent.h"
#include "Camera/WarCameraMode.h"

UWarCameraComponent::UWarCameraComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), bIsActive(true), FieldOfViewOffset(0.0f)
{

}

AActor* UWarCameraComponent::GetFocusActor() const
{
	return GetOwner();
}

void UWarCameraComponent::SetCurrentWarCameraMode(UWarCameraMode* NewWarCameraMode)
{
	CurrentWarCameraMode = NewWarCameraMode;
}

UWarCameraMode* UWarCameraComponent::GetDefaultFirstCameraMode() const
{
	return DefaultFirstCameraMode;
}

UWarCameraMode* UWarCameraComponent::GetDefaultThirdCameraMode() const
{
	return DefaultThirdCameraMode;
}

UWarCameraMode* UWarCameraComponent::GetDefaultAimDownSightCameraMode() const
{
	return DefaultAimDownSightCameraMode;
}

void UWarCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(!WarCameraModes.IsEmpty());
	if (bIsActive)
	{
		AddCameraMode();
	}
	FWarCameraModeViewData WarCameraModeViewData;
	EvaluateModes(DeltaTime, WarCameraModeViewData);
	if (APawn* Pawn = Cast<APawn>(GetFocusActor()))
	{
		if (APlayerController* PlayerController = Pawn->GetController<APlayerController>())
		{
			PlayerController->SetControlRotation(WarCameraModeViewData.ControlRotation);
		}
	}
	WarCameraModeViewData.FieldOfView = WarCameraModeViewData.FieldOfView + FieldOfViewOffset;
	FieldOfViewOffset = 0.0f;

	SetWorldLocationAndRotation(WarCameraModeViewData.Location, WarCameraModeViewData.Rotation);
	FieldOfView = WarCameraModeViewData.FieldOfView;
	DesiredView.Location = WarCameraModeViewData.Location;
	DesiredView.Rotation = WarCameraModeViewData.Rotation;
	DesiredView.FOV = WarCameraModeViewData.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
	if (IsXRHeadTrackedCamera())
	{
		Super::GetCameraView(DeltaTime, DesiredView);
	}
}

void UWarCameraComponent::AddCameraMode()
{
	if (!CurrentWarCameraMode)
	{
		return;
	}
	int32 WarCameraModesCount = WarCameraModes.Num();
	if ((WarCameraModesCount > 0) && (WarCameraModes[0] == CurrentWarCameraMode))
	{
		return;
	}
	int32 ExistingWarCameraModeIndex = INDEX_NONE;
	float ExistingWarCameraModeContribution = 1.0f;
	for (int32 WarCameraModeIndex = 0; WarCameraModeIndex < WarCameraModesCount; WarCameraModeIndex++)
	{
		if (WarCameraModes[WarCameraModeIndex] == CurrentWarCameraMode)
		{
			ExistingWarCameraModeIndex = WarCameraModeIndex;
			ExistingWarCameraModeContribution = ExistingWarCameraModeContribution * CurrentWarCameraMode->GetBlendWeight();
			break;
		}
		else
		{
			ExistingWarCameraModeContribution = ExistingWarCameraModeContribution * (1.0f - WarCameraModes[WarCameraModeIndex]->GetBlendWeight());
		}
	}
	if (ExistingWarCameraModeIndex != INDEX_NONE)
	{
		WarCameraModes.RemoveAt(ExistingWarCameraModeIndex);
		WarCameraModesCount--;
	}
	else
	{
		ExistingWarCameraModeContribution = 0.0f;
	}
	const bool bShouldBlend = ((CurrentWarCameraMode->GetBlendTime() > 0.0f) && (WarCameraModesCount > 0));
	const float BlendWeight = (bShouldBlend ? ExistingWarCameraModeContribution : 1.0f);
	CurrentWarCameraMode->SetBlendWeight(BlendWeight);
	WarCameraModes.Insert(CurrentWarCameraMode, 0);
	WarCameraModes.Last()->SetBlendWeight(1.0f);
	if (ExistingWarCameraModeIndex == INDEX_NONE)
	{
		CurrentWarCameraMode->OnActivation();
	}
}

void UWarCameraComponent::EvaluateModes(float DeltaTime, FWarCameraModeViewData& OutWarCameraModeViewData)
{
	if (!bIsActive)
	{
		return;
	}
	int32 WarCameraModesCount = WarCameraModes.Num();
	if (WarCameraModesCount <= 0)
	{
		return;
	}
	int32 ToBeRemovedCameraCount = 0;
	int32 ToBeRemovedCameraIndex = INDEX_NONE;
	for (int32 WarCameraModeIndex = 0; WarCameraModeIndex < WarCameraModesCount; WarCameraModeIndex++)
	{
		UWarCameraMode* WarCameraMode = WarCameraModes[WarCameraModeIndex];
		check(WarCameraMode);
		WarCameraMode->UpdateCameraModeData(DeltaTime);
		if (WarCameraMode->GetBlendWeight() >= 1.0f)
		{
			ToBeRemovedCameraIndex = (WarCameraModeIndex + 1);
			ToBeRemovedCameraCount = (WarCameraModesCount - ToBeRemovedCameraIndex);
			break;
		}
	}
	if (ToBeRemovedCameraCount > 0)
	{
		for (int32 WarCameraModeIndex = ToBeRemovedCameraIndex; WarCameraModeIndex < WarCameraModesCount; WarCameraModeIndex++)
		{
			UWarCameraMode* WarCameraMode = WarCameraModes[WarCameraModeIndex];
			check(WarCameraMode);
			WarCameraMode->OnDeactivation();
		}
		WarCameraModes.RemoveAt(ToBeRemovedCameraIndex, ToBeRemovedCameraCount);
	}
	WarCameraModesCount = WarCameraModes.Num();
	if (WarCameraModesCount <= 0)
	{
		return;
	}
	const UWarCameraMode* WarCameraMode = WarCameraModes[WarCameraModesCount - 1];
	check(WarCameraMode);
	OutWarCameraModeViewData = WarCameraMode->GetCameraModeViewData();
	const FWarCameraModeViewData& WarCameraModeViewDataAlpha = WarCameraMode->GetCameraModeViewData();
	for (int32 WarCameraModeIndex = (WarCameraModesCount - 2); WarCameraModeIndex >= 0; WarCameraModeIndex--)
	{
		WarCameraMode = WarCameraModes[WarCameraModeIndex];
		check(WarCameraMode);
		const FWarCameraModeViewData& WarCameraModeViewDataBeta = WarCameraMode->GetCameraModeViewData();
		Blend(const_cast<FWarCameraModeViewData&>(WarCameraModeViewDataAlpha), WarCameraModeViewDataBeta, WarCameraMode->GetBlendWeight());
	}
}

void UWarCameraComponent::Blend(FWarCameraModeViewData& WarCameraModeViewDataAlpha, const FWarCameraModeViewData& WarCameraModeViewDataBeta, float BlendWeightBeta)
{
	if (BlendWeightBeta <= 0.0f)
	{
		return;
	}
	else if (BlendWeightBeta >= 1.0f)
	{
		WarCameraModeViewDataAlpha = WarCameraModeViewDataBeta;
		return;
	}
	WarCameraModeViewDataAlpha.Location = FMath::Lerp(WarCameraModeViewDataAlpha.Location, WarCameraModeViewDataBeta.Location, BlendWeightBeta);
}
