// Personal Copyright
#include "ContextEffect/WarContextEffectsSubsystem.h"
#include "ContextEffect/WarContextEffectsLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(WarContextEffectsSubsystem)

void UWarContextEffectsSubsystem::SpawnContextEffects(const AActor* SpawningActor, USceneComponent* AttachToComponent, const FName AttachPoint, const FVector LocationOffset, const FRotator RotationOffset, FGameplayTag Effect, FGameplayTagContainer Contexts, TArray<UAudioComponent*>& AudioOut, TArray<UNiagaraComponent*>& NiagaraOut, FVector VisualEffectScale, float AudioVolume, float AudioPitch)
{
	if (TObjectPtr<UWarContextEffectsSet>* WarEffectsLibrariesSetPointer = WarActiveActorEffectsMap.Find(SpawningActor))
	{
		if (UWarContextEffectsSet* WarEffectsLibraries = *WarEffectsLibrariesSetPointer)
		{
			TArray<USoundBase*> TotalSounds;
			TArray<UNiagaraSystem*> TotalNiagaraSystems;
			for (UWarContextEffectsLibrary* WarEffectLibrary : WarEffectsLibraries->WarContextEffectsLibraries)
			{
				if (WarEffectLibrary && WarEffectLibrary->GetContextEffectsLibraryLoadState() == EWarContextEffectsLibraryLoadState::Loaded)
				{
					TArray<USoundBase*> Sounds;
					TArray<UNiagaraSystem*> NiagaraSystems;
					WarEffectLibrary->GetEffects(Effect, Contexts, Sounds, NiagaraSystems);
					TotalSounds.Append(Sounds);
					TotalNiagaraSystems.Append(NiagaraSystems);
				}
				else if (WarEffectLibrary && WarEffectLibrary->GetContextEffectsLibraryLoadState() == EWarContextEffectsLibraryLoadState::Unloaded)
				{
					WarEffectLibrary->LoadEffects();
				}
			}
			for (USoundBase* Sound : TotalSounds)
			{
				UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(Sound, AttachToComponent, AttachPoint, LocationOffset, RotationOffset, EAttachLocation::KeepRelativeOffset, false, AudioVolume, AudioPitch, 0.0f, nullptr, nullptr, true);
				AudioOut.Add(AudioComponent);
			}
			for (UNiagaraSystem* NiagaraSystem : TotalNiagaraSystems)
			{
				UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, AttachToComponent, AttachPoint, LocationOffset, RotationOffset, VisualEffectScale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true, true);
				NiagaraOut.Add(NiagaraComponent);
			}
		}
	}
}

bool UWarContextEffectsSubsystem::GetContextFromSurfaceType(TEnumAsByte<EPhysicalSurface> PhysicalSurface, FGameplayTag& Context)
{
	if (const UWarContextEffectsSettings* ProjectSettings = GetDefault<UWarContextEffectsSettings>())
	{
		if (const FGameplayTag* GameplayTagPointer = ProjectSettings->SurfaceTypeToContextMap.Find(PhysicalSurface))
		{
			Context = *GameplayTagPointer;
		}
	}
	return Context.IsValid();
}

void UWarContextEffectsSubsystem::LoadAndAddContextEffectsLibraries(AActor* OwningActor, TSet<TSoftObjectPtr<UWarContextEffectsLibrary>> WarContextEffectsLibraries)
{
	if (OwningActor == nullptr || WarContextEffectsLibraries.Num() <= 0)
	{
		return;
	}
	UWarContextEffectsSet* WarContextEffectsLibrariesSet = NewObject<UWarContextEffectsSet>(this);
	for (const TSoftObjectPtr<UWarContextEffectsLibrary>& WarContextEffectsSoftObject : WarContextEffectsLibraries)
	{
		if (UWarContextEffectsLibrary* WarContextEffectsLibrary = WarContextEffectsSoftObject.LoadSynchronous())
		{
			WarContextEffectsLibrary->LoadEffects();
			WarContextEffectsLibrariesSet->WarContextEffectsLibraries.Add(WarContextEffectsLibrary);
		}
	}
	WarActiveActorEffectsMap.Emplace(OwningActor, WarContextEffectsLibrariesSet);
}

void UWarContextEffectsSubsystem::UnloadAndRemoveContextEffectsLibraries(AActor* OwningActor)
{
	if (OwningActor == nullptr)
	{
		return;
	}
	WarActiveActorEffectsMap.Remove(OwningActor);
}
