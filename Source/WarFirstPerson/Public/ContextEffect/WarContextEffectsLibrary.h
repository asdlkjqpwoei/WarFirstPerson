// Personal Copyright
#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "WarContextEffectsLibrary.generated.h"

class UNiagaraSystem;
class USoundBase;

UENUM()
enum class EWarContextEffectsLibraryLoadState : uint8
{
	Unloaded = 0,
	Loading = 1,
	Loaded = 2
};

USTRUCT(BlueprintType)
struct WARFIRSTPERSON_API FWarContextEffects
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Context;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowedClasses = "/Script/Engine.SoundBase, /Script/Niagara.NiagaraSystem"))
	TArray<FSoftObjectPath> Effects;
};

UCLASS()
class WARFIRSTPERSON_API UWarActiveContextEffects : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	FGameplayTag EffectTag;

	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainer Context;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<USoundBase>> Sounds;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UNiagaraSystem>> NiagaraSystems;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FWarContextEffectLibraryLoadingComplete, TArray<UWarActiveContextEffects*>, WarActiveContextEffects);

/**
 * 
 */
UCLASS(BlueprintType)
class WARFIRSTPERSON_API UWarContextEffectsLibrary : public UObject
{
	GENERATED_BODY()
public:
	EWarContextEffectsLibraryLoadState GetContextEffectsLibraryLoadState();

	UFUNCTION(BlueprintCallable)
	void GetEffects(const FGameplayTag Effect, const FGameplayTagContainer Context, TArray<USoundBase*>& Sounds, TArray<UNiagaraSystem*>& NiagaraSystems);

	UFUNCTION(BlueprintCallable)
	void LoadEffects();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FWarContextEffects> WarContextEffects;

protected:
	void LoadEffectsInternal();
	void WarContextEffectLibraryLoadingComplete(TArray<UWarActiveContextEffects*> WarActiveContextEffects);

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UWarActiveContextEffects>> WarActiveContextEffects;

	UPROPERTY(Transient)
	EWarContextEffectsLibraryLoadState WarEffectsLoadState = EWarContextEffectsLibraryLoadState::Unloaded;
};
