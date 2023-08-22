// Personal Copyright
#include "Camera/WarCameraMode.h"
#include "Camera/WarCameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(WarCameraMode)

FWarCameraModeViewData::FWarCameraModeViewData() : Location(ForceInit), Rotation(ForceInit), ControlRotation(ForceInit), FieldOfView(80.0f)
{

}

UWarCameraMode::UWarCameraMode()
{
	ViewPitchMin = -89.0f;
	ViewPitchMax = 89.0f;

	BlendTime = 0.5f;
	WarCameraModeBlendType = EWarCameraModeBlendType::EaseOut;
	BlendExponent = 4.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;
}

UWarCameraComponent* UWarCameraMode::GetWarCameraComponent() const
{
	return CastChecked<UWarCameraComponent>(GetOuter());
}

AActor* UWarCameraMode::GetOwnerActor() const
{
	const UWarCameraComponent* WarCameraComponent = GetWarCameraComponent();
	return WarCameraComponent->GetOwnerActor();
}

float UWarCameraMode::GetBlendTime() const
{
	return BlendTime;
}

float UWarCameraMode::GetBlendWeight() const
{
	return BlendWeight;
}

void UWarCameraMode::SetBlendWeight(const float& Weight)
{
	BlendWeight = FMath::Clamp(Weight, 0.0f, 1.0f);
	const float Exponent = (BlendExponent > 0.0f) ? (1.0f / BlendExponent) : 1.0f;
	switch (WarCameraModeBlendType)
	{
		case EWarCameraModeBlendType::Linear:
			BlendAlpha = BlendWeight;
			break;
		case EWarCameraModeBlendType::EaseIn:
			BlendAlpha = FMath::InterpEaseIn(0.0f, 1.0f, BlendWeight, Exponent);
			break;
		case EWarCameraModeBlendType::EaseOut:
			BlendAlpha = FMath::InterpEaseOut(0.0f, 1.0f, BlendWeight, Exponent);
			break;
		case EWarCameraModeBlendType::EaseInOut:
			BlendAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, BlendWeight, Exponent);
			break;
		default:
			checkf(false, TEXT("WarCameraMode SetBlendWeight: Invalid blend type [%d]\n"), (uint8)WarCameraModeBlendType)
			break;
	}
}

const FWarCameraModeViewData& UWarCameraMode::GetCameraModeViewData() const
{
	return WarCameraModeViewData;
}

void UWarCameraMode::UpdateCameraModeData(float DeltaTime)
{
	FVector PivotLocation = GetOwnerActorPivotLocation();
	FRotator PivotRotation = GetOwnerActorPivotRotation();
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);
	WarCameraModeViewData.Location = PivotLocation;
	WarCameraModeViewData.Rotation = PivotRotation;
	WarCameraModeViewData.ControlRotation = WarCameraModeViewData.Rotation;
	if (BlendTime > 0.0f)
	{
		BlendAlpha = BlendAlpha + (DeltaTime / BlendTime);
		BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
	}
	else
	{
		BlendAlpha = 1.0f;
	}
	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
	switch (WarCameraModeBlendType)
	{
		case EWarCameraModeBlendType::Linear:
			BlendWeight = BlendAlpha;
			break;
		case EWarCameraModeBlendType::EaseIn:
			BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
			break;
		case EWarCameraModeBlendType::EaseOut:
			BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
			break;
		default:
			checkf(false, TEXT("WarCameraMode UpdateBlending: Invalid blend type [%d]\n"), (uint8)WarCameraModeBlendType);
			break;
	}
}

EWarCameraType UWarCameraMode::GetWarCameraType() const
{
	return WarCameraType;
}

UWorld* UWarCameraMode::GetWorld() const
{
	return HasAnyFlags(RF_ClassDefaultObject) ? nullptr : GetOuter()->GetWorld();
}

void UWarCameraMode::OnActivation()
{

}

void UWarCameraMode::OnDeactivation()
{

}

FVector UWarCameraMode::GetOwnerActorPivotLocation() const
{
	const AActor* Actor = GetOwnerActor();
	check(Actor);
	if (const APawn* Pawn = Cast<APawn>(Actor))
	{
		if (const ACharacter* Character = Cast<ACharacter>(Pawn))
		{
			const ACharacter* CharacterClassDefaultObject = Character->GetClass()->GetDefaultObject<ACharacter>();
			check(CharacterClassDefaultObject);
			const UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
			check(CapsuleComponent);
			const UCapsuleComponent* CapsuleComponentClassDefaultObject = CharacterClassDefaultObject->GetCapsuleComponent();
			check(CapsuleComponent);
			const float DefaultHalfHeight = CapsuleComponentClassDefaultObject->GetUnscaledCapsuleHalfHeight();
			const float ActualHalfHeight = CapsuleComponent->GetUnscaledCapsuleHalfHeight();
			const float HeightAdjustment = (DefaultHalfHeight - ActualHalfHeight) + CharacterClassDefaultObject->BaseEyeHeight;
			return Character->GetActorLocation() + (FVector::UpVector * HeightAdjustment);
		}
		return Pawn->GetPawnViewLocation();
	}
	return Actor->GetActorLocation();
}

FRotator UWarCameraMode::GetOwnerActorPivotRotation() const
{
	const AActor* Actor = GetOwnerActor();
	check(Actor);
	if (const APawn* Pawn = Cast<APawn>(Actor))
	{
		return Pawn->GetViewRotation();
	}
	return Actor->GetActorRotation();
}
