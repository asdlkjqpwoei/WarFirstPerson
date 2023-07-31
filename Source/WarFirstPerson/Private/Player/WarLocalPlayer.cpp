// Personal Copyright
#include "Player/WarLocalPlayer.h"
#include "Settings/WarGameUserSettings.h"

UWarLocalPlayer::UWarLocalPlayer()
{
}

UWarGameUserSettings* UWarLocalPlayer::GetLocalSettings() const
{
	return UWarGameUserSettings::Get();
}
