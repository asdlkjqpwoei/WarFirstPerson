// Personal Copyright

#include "Settings/WarGameUserSettings.h"
#include "Input/WarPlayerMappableInputConfigPair.h"

UWarGameUserSettings::UWarGameUserSettings()
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && FSlateApplication::IsInitialized())
	{
		//OnApplicationActivationStateChangedHandle = FSlateApplication::Get().OnApplicationActivationStateChanged().AddUObject(this, &ThisClass::OnAppActivationStateChanged);
	}
	SetToDefaults();
}

UWarGameUserSettings* UWarGameUserSettings::Get()
{
	return GEngine ? CastChecked<UWarGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

float UWarGameUserSettings::GetEffectiveFrameRateLimit()
{
	// TODO
	return 0.0f;
}

void UWarGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();
}

float UWarGameUserSettings::GetDisplayGamma() const
{
	return DisplayGamma;
}

void UWarGameUserSettings::SetDisplayGamma(float InGamma)
{
	DisplayGamma = InGamma;
	ApplyDisplayGamma();
}

float UWarGameUserSettings::GetFrameRateLimitOnBattery() const
{
	return FrameRateLimitOnBattery;
}

void UWarGameUserSettings::SetFrameRateLimitOnBattery(float NewLimitFPS)
{
	FrameRateLimitOnBattery = NewLimitFPS;
	UpdateEffectiveFrameRateLimit();
}

float UWarGameUserSettings::GetFrameRateLimitInMenu() const
{
	return FrameRateLimitInMenu;
}

void UWarGameUserSettings::SetFrameRateLimitInMenu(float NewLimitFPS)
{
	FrameRateLimitInMenu = NewLimitFPS;
	UpdateEffectiveFrameRateLimit();
}

float UWarGameUserSettings::GetFrameRateLimitWhenBackgrounded() const
{
	return FrameRateLimitWhenBackgrounded;
}

void UWarGameUserSettings::SetFrameRateLimitWhenBackgrounded(float NewLimitFPS)
{
	FrameRateLimitWhenBackgrounded = NewLimitFPS;
	UpdateEffectiveFrameRateLimit();
}

float UWarGameUserSettings::GetFrameRateLimitAlways() const
{
	return GetFrameRateLimit();
}

void UWarGameUserSettings::SetFrameRateLimitAlways(float NewLimitFPS)
{
	SetFrameRateLimit(NewLimitFPS);
	UpdateEffectiveFrameRateLimit();
}

void UWarGameUserSettings::RegisterInputConfig(ECommonInputType Type, const UPlayerMappableInputConfig* NewConfig, const bool bIsActive)
{

}

void UWarGameUserSettings::ApplyDisplayGamma()
{
	if (GEngine)
	{
		GEngine->DisplayGamma = DisplayGamma;
	}
}

void UWarGameUserSettings::UpdateEffectiveFrameRateLimit()
{
	if (!IsRunningDedicatedServer())
	{
		SetFrameRateLimitCVar(GetEffectiveFrameRateLimit());
	}
}
