// Personal Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WarContextEffectComponent.generated.h"

class UWarContextEffectsLibrary;
class UNiagaraComponent;
struct FGameplayTag;
struct FGameplayTagContainer;

UCLASS(ClassGroup=(Custom), hidecategories = (Variable, Tags, ComponentTick, ComponentReplication, Activation, Cooking, AssetUserData, Collision), CollapseCategories, meta = (BlueprintSpawnableComponent))
class WARFIRSTPERSON_API UWarContextEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWarContextEffectComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AnimMotionEffect(const FName Bone, const FGameplayTag MotionEffect, USceneComponent* StaticMeshComponent, const FVector LocationOffset, const FRotator RotationOffset, const UAnimSequenceBase* AnimationSequence, const bool bHitSuccess, const FHitResult HitResult, FGameplayTagContainer GameplayTagContainerContext, FVector VisualEffectScale = FVector(1), float AudioVolume = 1, float AudioPitch = 1);

	UFUNCTION(BlueprintCallable)
	void UpdateEffectContexts(FGameplayTagContainer NewEffectContexts);

	UFUNCTION(BlueprintCallable)
	void UpdateLibraries(TSet<TSoftObjectPtr<UWarContextEffectsLibrary>> NewWarContextEffectsLibraries);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bConvertPhysicalSurfaceToContext = true;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer DefaultEffectContexts;

	UPROPERTY(EditAnywhere)
	TSet<TSoftObjectPtr<UWarContextEffectsLibrary>> DefaultWarContextEffectsLibraries;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(Transient)
	FGameplayTagContainer CurrentContexts;

	UPROPERTY(Transient)
	TSet<TSoftObjectPtr<UWarContextEffectsLibrary>> CurrentWarContextEffectsLibraries;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UAudioComponent>> ActiveAudioComponents;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> ActiveNiagaraComponents;
};
