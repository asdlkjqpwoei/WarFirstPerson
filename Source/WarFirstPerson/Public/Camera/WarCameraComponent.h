// Personal Copyright
#pragma once
#include "Camera/CameraComponent.h"
#include "WarCameraComponent.generated.h"

class UWarCameraMode;
struct FWarCameraModeViewData;

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API UWarCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	UWarCameraComponent(const FObjectInitializer& ObjectInitializer);

	virtual AActor* GetFocusActor() const;
	void SetCurrentWarCameraMode(UWarCameraMode* NewWarCameraMode);

	UFUNCTION(BlueprintPure, Category = "War|Camera")
	static UWarCameraComponent* FindCameraComponent(const AActor* Actor) {return (Actor ? Actor->FindComponentByClass<UWarCameraComponent>() : nullptr); }

	UFUNCTION(BlueprintCallable, Category = "War|Camera")
	UWarCameraMode* GetDefaultFirstCameraMode() const;

	UFUNCTION(BlueprintCallable, Category = "War|Camera")
	UWarCameraMode* GetDefaultThirdCameraMode() const;

	UFUNCTION(BlueprintCallable, Category = "War|Camera")
	UWarCameraMode* GetDefaultAimDownSightCameraMode() const;

protected:
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;
	void AddCameraMode();
	void EvaluateModes(float DeltaTime, FWarCameraModeViewData& OutWarCameraModeViewData);
	void Blend(FWarCameraModeViewData& WarCameraModeViewDataAlpha, const FWarCameraModeViewData& WarCameraModeViewDataBeta, float BlendWeightBeta);

	bool bIsActive;
	float FieldOfViewOffset;
	TObjectPtr<UWarCameraMode> CurrentWarCameraMode;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "War|Camera")
	TArray<TObjectPtr<UWarCameraMode>> WarCameraModes;

	UPROPERTY(BlueprintReadOnly, BlueprintGetter = GetDefaultFirstCameraMode, EditDefaultsOnly, Category = "War|Camera")
	TObjectPtr<UWarCameraMode> DefaultFirstCameraMode;

	UPROPERTY(BlueprintReadOnly, BlueprintGetter = GetDefaultThirdCameraMode, EditDefaultsOnly, Category = "War|Camera")
	TObjectPtr<UWarCameraMode> DefaultThirdCameraMode;

	UPROPERTY(BlueprintReadOnly, BlueprintGetter = GetDefaultAimDownSightCameraMode, EditDefaultsOnly, Category = "War|Camera")
	TObjectPtr<UWarCameraMode> DefaultAimDownSightCameraMode;
};
