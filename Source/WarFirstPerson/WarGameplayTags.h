// Personal Copyright
#pragma once

#include "Containers/UnrealString.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

struct FWarGameplayTags
{
public:
	static const FWarGameplayTags& Get() { return WarGameplayTags; }
	static void InitializeNativeTags();
	static FGameplayTag FindTagByString(FString TagString, bool bMatchPartialString = false);
#if UE_BUILD_DEVELOPMENT
	FGameplayTag InputToggleDebugInformation;
#endif
	FGameplayTag InputMove;
	FGameplayTag InputTurn;
	FGameplayTag InputCrouch;
	FGameplayTag InputJump;
	FGameplayTag InputInteract;
	FGameplayTag InputFire;
	FGameplayTag InputAimDownSight;
	FGameplayTag InputEndAimDownSight;
	FGameplayTag InputCycleActiveWeaponForward;
	FGameplayTag InputCycleActiveWeaponBackward;

	FGameplayTag GameplayEventDeath;

	FGameplayTag StatusCrouch;
	FGameplayTag StatusDying;
	FGameplayTag StatusDead;

	FGameplayTag MovementWalking;
	FGameplayTag MovementFalling;

	FGameplayTag HitZoneWeakSpot;

	FGameplayTag ItemTypeWeaponPistol;
	FGameplayTag ItemTypeWeaponRifle;

protected:
	void AddAllTagsIntoManager(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

private:
	static FWarGameplayTags WarGameplayTags;
};