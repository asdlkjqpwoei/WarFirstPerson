// Personal Copyright
#pragma once
#include "UObject/NoExportTypes.h"
#include "WarCameraMode.generated.h"

class UWarCameraComponent;

UENUM(BlueprintType)
enum class EWarCameraType : uint8
{
	FirstPerson,
	ThirdPerson
};

UENUM(BlueprintType)
enum class EWarCameraModeBlendType : uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut
};

struct FWarCameraModeViewData
{
	FWarCameraModeViewData();
	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API UWarCameraMode : public UObject
{
	GENERATED_BODY()
public:
	UWarCameraMode();
	UWarCameraComponent* GetWarCameraComponent() const;
	AActor* GetFocusActor() const;
	float GetBlendTime() const;
	float GetBlendWeight() const;
	void SetBlendWeight(const float& Weight);
	const FWarCameraModeViewData& GetCameraModeViewData() const;
	virtual UWorld* GetWorld() const override;

	void UpdateCameraModeData(float DeltaTime);
	virtual void OnActivation();
	virtual void OnDeactivation();

protected:
	virtual FVector GetFocusActorPivotLocation() const;
	virtual FRotator GetFocusActorPivotRotation() const;

	FWarCameraModeViewData WarCameraModeViewData;
	
	UPROPERTY(EditDefaultsOnly, Category = "War|Camera Type")
	EWarCameraType WarCameraType;

	//UPROPERTY(EditDefaultsOnly, Category = "War|Camera View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	//float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "War|Camera View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "War|Camera View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category = "War|Blending")
	float BlendTime;

	UPROPERTY(EditDefaultsOnly, Category = "War|Bleanding")
	EWarCameraModeBlendType WarCameraModeBlendType;

	UPROPERTY(EditDefaultsOnly, Category = "War|Blending")
	float BlendExponent;

	float BlendAlpha;

	float BlendWeight;
};
