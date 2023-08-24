// Personal Copyright
#pragma once
#include "UObject/NoExportTypes.h"
#include "WarCameraMode.generated.h"

class UWarCameraComponent;

UENUM(BlueprintType)
enum class EWarCameraType : uint8
{
	FirstPerson,
	ThirdPerson,
	AimDownSight,
	Invalid
};

UENUM(BlueprintType)
enum class EWarCameraModeBlendType : uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut
};

USTRUCT(BlueprintType)
struct FWarCameraModeViewData
{
	GENERATED_BODY();
	FWarCameraModeViewData();
	UPROPERTY(VisibleDefaultsOnly, Category = "War|Camera Mode Data")
	FVector CurrentLocation;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "War|Camera Mode Data")
	FRotator CurrentRotation;

	UPROPERTY(VisibleDefaultsOnly, Category = "War|Camera Mode Data")
	FRotator CurrentControlRotation;

	UPROPERTY(VisibleDefaultsOnly, Category = "War|Camera Mode Data")
	float CurrentFieldOfView;
};

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class WARFIRSTPERSON_API UWarCameraMode : public UObject
{
	GENERATED_BODY()
public:
	UWarCameraMode();
	UWarCameraComponent* GetWarCameraComponent() const;
	AActor* GetOwnerActor() const;
	float GetBlendTime() const;
	float GetBlendWeight() const;
	void SetBlendWeight(const float& Weight);
	const FWarCameraModeViewData& GetCameraModeViewData() const;
	virtual void UpdateCameraModeData(float DeltaTime);
	EWarCameraType GetWarCameraType() const;

	virtual UWorld* GetWorld() const override;
	virtual void OnActivation();
	virtual void OnDeactivation();

protected:
	virtual FVector GetOwnerActorPivotLocation() const;
	virtual FRotator GetOwnerActorPivotRotation() const;

	UPROPERTY(EditDefaultsOnly, Category = "War|Camera Mode View")
	FWarCameraModeViewData WarCameraModeViewData;
	
	UPROPERTY(EditDefaultsOnly, Category = "War|Camera Type")
	EWarCameraType WarCameraType;

	UPROPERTY(EditDefaultsOnly, Category = "War|Camera View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "War|Camera View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "War|Camera View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	UPROPERTY(Transient)
	uint32 bResetInterpolation:1;

	UPROPERTY(EditDefaultsOnly, Category = "War|Blending")
	float BlendTime;

	UPROPERTY(EditDefaultsOnly, Category = "War|Bleanding")
	EWarCameraModeBlendType WarCameraModeBlendType;

	UPROPERTY(EditDefaultsOnly, Category = "War|Blending")
	float BlendExponent;

	float BlendAlpha;

	float BlendWeight;
};
