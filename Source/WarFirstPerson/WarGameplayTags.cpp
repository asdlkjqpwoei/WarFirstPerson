// Personal Copyright
#include "WarGameplayTags.h"
#include "WarLogChannels.h"
#include "GameplayTagsManager.h"

FWarGameplayTags FWarGameplayTags::WarGameplayTags;

void FWarGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
	WarGameplayTags.AddAllTagsIntoManager(GameplayTagsManager);
	GameplayTagsManager.DoneAddingNativeTags();
}

void FWarGameplayTags::AddAllTagsIntoManager(UGameplayTagsManager& Manager)
{
#if UE_BUILD_DEVELOPMENT
	AddTag(InputToggleDebugInformation, "Input.Action.ToggleDebugInformation", "Toggle debug information weidget.");
#endif
	AddTag(InputMove, "Input.Action.Move", "Move input");
	AddTag(InputTurn, "Input.Action.Turn", "Turn input");
	AddTag(InputCrouch, "Input.Action.Crouch", "Crouch input");
	AddTag(InputJump, "Input.Action.Jump", "Jump input");
	AddTag(InputInteract, "Input.Action.Interact" , "Interact input");
	AddTag(InputFire, "Input.Action.Fire", "Weapon fire input");
	AddTag(InputAimDownSight, "Input.Action.AimDownSight", "Weapon aim down sight input");
	AddTag(InputEndAimDownSight, "Input.Aciton.EndAimDownSight", "Weapon end aim down sight input.");
	AddTag(InputCycleActiveWeaponForward, "Input.Action.CycleActiveWeaponForward", "Cycle active weapon forward input");
	AddTag(InputCycleActiveWeaponBackward, "Input.Action.CycleActiveWeaponBackward", "Cycle active weapon backward input");
	AddTag(HitZoneWeakSpot, "HitZone.WeakSpot", "Weak spot hitted");
	AddTag(ItemTypeWeaponPistol, "Item.Weapon.Pistol", "Weapon Pistol");
	AddTag(ItemTypeWeaponRifle, "Item.Weapon.Rifle", "Weapon Rifle");
}

void FWarGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native)")) + FString("TagComment"));
}

FGameplayTag FWarGameplayTags::FindTagByString(FString TagString, bool bMatchPartialString)
{
	const UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
	FGameplayTag GameplayTag = GameplayTagsManager.RequestGameplayTag(FName(*TagString), false);
	if (!GameplayTag.IsValid() && bMatchPartialString)
	{
		FGameplayTagContainer AllTags;
		GameplayTagsManager.RequestAllGameplayTags(AllTags, true);
		for (const FGameplayTag TestTag : AllTags)
		{
			if (TestTag.ToString().Contains(TagString))
			{
				UE_LOG(LogWar, Display, TEXT("Could not find exact match for tag[%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
				GameplayTag = TestTag;
				break;
			}
		}
	}
	return GameplayTag;
}
