// Personal Copyright
#pragma once
#include "Camera/CameraComponent.h"
#include "WarCameraComponent.generated.h"

enum class EWarCameraType : uint8;
class UWarCameraMode;
class UWarFirstPersonCameraMode;
class UWarThirdPersonCameraMode;
class UWarAimDownSightCameraMode;
struct FGameplayTag;
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

	virtual AActor* GetOwnerActor() const;
	void SetCurrentWarCameraMode(UWarCameraMode* NewWarCameraMode);
	void GetBlendInfo(float& OutWeightOfTopCamera, EWarCameraType& OutTypeOfTopCamera) const;
	TArray<const AActor*> GetIgnoredActorsForCameraPenetration() const;
	TOptional<AActor*> GetCameraPreventPenetrationTarget() const;
	void OnCameraPenetratingTarget();

	UFUNCTION(BlueprintPure, Category = "War|Camera")
	static UWarCameraComponent* FindCameraComponent(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "War|Camera")
	TSubclassOf<UWarFirstPersonCameraMode> GetDefaultFirstPersonCameraMode() const;

	UFUNCTION(BlueprintCallable, Category = "War|Camera")
	TSubclassOf<UWarThirdPersonCameraMode> GetDefaultThirdPersonCameraMode() const;

	UFUNCTION(BlueprintCallable, Category = "War|Camera")
	TSubclassOf<UWarAimDownSightCameraMode> GetDefaultAimDownSightCameraMode() const;

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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, BlueprintGetter = GetDefaultFirstPersonCameraMode, Category = "War|Camera")
	TSubclassOf<UWarFirstPersonCameraMode> DefaultFirstPersonCameraMode;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, BlueprintGetter = GetDefaultThirdPersonCameraMode, Category = "War|Camera")
	TSubclassOf<UWarThirdPersonCameraMode> DefaultThirdPersonCameraMode;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, BlueprintGetter = GetDefaultAimDownSightCameraMode, Category = "War|Camera")
	TSubclassOf<UWarAimDownSightCameraMode> DefaultAimDownSightCameraMode;
};
