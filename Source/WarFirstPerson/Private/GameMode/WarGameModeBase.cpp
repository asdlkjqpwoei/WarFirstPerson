// Personal Copyright
#include "GameMode/WarGameModeBase.h"
#include "Character/WarPawnComponent.h"
#include "Character/WarPawnData.h"
#include "Player/WarPlayerState.h"
#include "WarFirstPerson/WarLogChannels.h"
#include "System/WarAssetManager.h"


AWarGameModeBase::AWarGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AWarGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

APawn* AWarGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;
	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (UWarPawnComponent* WarPawnComponent = UWarPawnComponent::FindPawnComponent(SpawnedPawn))
			{
				if (const UWarPawnData* WarPawnData = GetWarPawnDataForController(NewPlayer))
				{
					WarPawnComponent->SetPawnData(WarPawnData);
				}
				else
				{
					UE_LOG(LogWar, Error, TEXT("Game mode was unable to set WarPawnData on the spawned pawn [%s]."), *GetNameSafe(SpawnedPawn));
				}
			}
			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
		else
		{
			UE_LOG(LogWar, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogWar, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}
	return nullptr;
}

const UWarPawnData* AWarGameModeBase::GetWarPawnDataForController(const AController* InController) const
{
	if (InController != nullptr)
	{
		if (const AWarPlayerState* WarPlayerState = InController->GetPlayerState<AWarPlayerState>())
		{
			if (const UWarPawnData* WarPawnData = WarPlayerState->GetWarPawnData<UWarPawnData>())
			{
				return WarPawnData;
			}
		}
	}
	return UWarAssetManager::Get().GetDefaultWarPawnData();
}
