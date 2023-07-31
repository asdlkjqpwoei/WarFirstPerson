// Personal Copyright
#include "ContextEffect/WarContextEffectsLibrary.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(WarContextEffectsLibrary)

EWarContextEffectsLibraryLoadState UWarContextEffectsLibrary::GetContextEffectsLibraryLoadState()
{
	return WarEffectsLoadState;
}

void UWarContextEffectsLibrary::GetEffects(const FGameplayTag Effect, const FGameplayTagContainer Context, TArray<USoundBase*>& Sounds, TArray<UNiagaraSystem*>& NiagaraSystems)
{
	if (Effect.IsValid() && Context.IsValid() && WarEffectsLoadState == EWarContextEffectsLibraryLoadState::Loaded)
	{
		for (const TObjectPtr<UWarActiveContextEffects> WarActiveContextEffect : WarActiveContextEffects)
		{
			if (Effect.MatchesTagExact(WarActiveContextEffect->EffectTag) && Context.HasAllExact(WarActiveContextEffect->Context) && (WarActiveContextEffect->Context.IsEmpty() == Context.IsEmpty()))
			{
				Sounds.Append(WarActiveContextEffect->Sounds);
				NiagaraSystems.Append(WarActiveContextEffect->NiagaraSystems);
			}
		}
	}
}

void UWarContextEffectsLibrary::LoadEffects()
{
	if (WarEffectsLoadState != EWarContextEffectsLibraryLoadState::Loading)
	{
		WarEffectsLoadState = EWarContextEffectsLibraryLoadState::Loading;
		WarActiveContextEffects.Empty();
		LoadEffectsInternal();
	}
}

void UWarContextEffectsLibrary::LoadEffectsInternal()
{
	TArray<FWarContextEffects> LocalWarContextEffects = WarContextEffects;
	TArray<UWarActiveContextEffects*> WarActiveContextEffectsArray;
	for (const FWarContextEffects& WarContextEffect : LocalWarContextEffects)
	{
		if (WarContextEffect.EffectTag.IsValid() && WarContextEffect.Context.IsValid())
		{
			UWarActiveContextEffects* NewWarActiveContextEffects = NewObject<UWarActiveContextEffects>(this);
			NewWarActiveContextEffects->EffectTag = WarContextEffect.EffectTag;
			NewWarActiveContextEffects->Context = WarContextEffect.Context;
			for (const FSoftObjectPath& Effect : WarContextEffect.Effects)
			{
				if (UObject* Object = Effect.TryLoad())
				{
					if(USoundBase* SoundBase = Cast<USoundBase>(Object))
					{
						NewWarActiveContextEffects->Sounds.Add(SoundBase);
					}
				}
				else if (Object->IsA(UNiagaraSystem::StaticClass()))
				{
					if (UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(Object))
					{
						NewWarActiveContextEffects->NiagaraSystems.Add(NiagaraSystem);
					}
				}
			}
			WarActiveContextEffectsArray.Add(NewWarActiveContextEffects);
		}
	}
	this->WarContextEffectLibraryLoadingComplete(WarActiveContextEffectsArray);
}

void UWarContextEffectsLibrary::WarContextEffectLibraryLoadingComplete(TArray<UWarActiveContextEffects*> ActiveContextEffects)
{
	WarEffectsLoadState = EWarContextEffectsLibraryLoadState::Loaded;
	ActiveContextEffects.Append(ActiveContextEffects);
}
