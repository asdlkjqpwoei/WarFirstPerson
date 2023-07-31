// Personal Copyright
#pragma once
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h" 
#include "Subsystems/WorldSubsystem.h"
#include "WarContextEffectsSubsystem.generated.h"

class AActor;
class UWarContextEffectsLibrary;
class UNiagaraComponent;
class USceneComponent;
enum EPhysicalSurface : int;
struct FGameplayTag;
struct FGameplayTagContainer;

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "War|ContextEffects"))
class WARFIRSTPERSON_API UWarContextEffectsSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(config, EditAnywhere)
	TMap<TEnumAsByte<EPhysicalSurface>, FGameplayTag> SurfaceTypeToContextMap;
};

UCLASS()
class WARFIRSTPERSON_API UWarContextEffectsSet : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	TSet<TObjectPtr<UWarContextEffectsLibrary>> WarContextEffectsLibraries;
};
/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API UWarContextEffectsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "War|ContextEffects")
	void SpawnContextEffects(const AActor* SpawningActor, USceneComponent* AttachToComponent, const FName AttachPoint, const FVector LocationOffset, const FRotator RotationOffset, FGameplayTag Effect, FGameplayTagContainer Contexts, TArray<UAudioComponent*>& AudioOut, TArray<UNiagaraComponent*>& NiagaraOut, FVector VisualEffectScale = FVector(1), float AudioVolume = 1, float AudioPitch = 1);

	UFUNCTION(BlueprintCallable, Category = "War|ContextEffects")
	bool GetContextFromSurfaceType(TEnumAsByte<EPhysicalSurface> PhysicalSurface, FGameplayTag& Context);

	UFUNCTION(BlueprintCallable, Category = "War|ContextEffects")
	void LoadAndAddContextEffectsLibraries(AActor* OwningActor, TSet<TSoftObjectPtr<UWarContextEffectsLibrary>> WarContextEffectsLibraries);

	UFUNCTION(BlueprintCallable, Category = "War|ContextEffects")
	void UnloadAndRemoveContextEffectsLibraries(AActor* OwningActor);

private:
	UPROPERTY(Transient)
	TMap<TObjectPtr<AActor>, TObjectPtr<UWarContextEffectsSet>> WarActiveActorEffectsMap;
};
