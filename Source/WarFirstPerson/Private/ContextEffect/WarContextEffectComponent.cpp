// Personal Copyright
#include "ContextEffect/WarContextEffectComponent.h"
#include "ContextEffect/WarContextEffectsSubsystem.h"
#include "GameplayTagContainer.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
// Sets default values for this component's properties
UWarContextEffectComponent::UWarContextEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
}

// Called every frame
void UWarContextEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWarContextEffectComponent::AnimMotionEffect(const FName Bone, const FGameplayTag MotionEffect, USceneComponent* StaticMeshComponent, const FVector LocationOffset, const FRotator RotationOffset, const UAnimSequenceBase* AnimationSequence, const bool bHitSuccess, const FHitResult HitResult, FGameplayTagContainer GameplayTagContainerContexts, FVector VisualEffectScale, float AudioVolume, float AudioPitch)
{
	TArray<UAudioComponent*> AudioComponentsToBeAdded;
	TArray<UNiagaraComponent*> NiagaraComponentsToBeAdded;
	FGameplayTagContainer TotalContexts;
	TotalContexts.AppendTags(GameplayTagContainerContexts);
	TotalContexts.AppendTags(CurrentContexts);
	if (bConvertPhysicalSurfaceToContext)
	{
		TWeakObjectPtr<UPhysicalMaterial> PhysicalSurfaceTypePointer = HitResult.PhysMaterial;
		if (PhysicalSurfaceTypePointer.IsValid())
		{
			TEnumAsByte<EPhysicalSurface> PhysicalSurfaceType = PhysicalSurfaceTypePointer->SurfaceType;
			if (const UWarContextEffectsSettings* WarContextEffectsSettings = GetDefault<UWarContextEffectsSettings>())
			{
				if (const FGameplayTag* GameplayTagSurfaceContextPointer = WarContextEffectsSettings->SurfaceTypeToContextMap.Find(PhysicalSurfaceType))
				{
					FGameplayTag GameplayTagSurfaceContext = *GameplayTagSurfaceContextPointer;
					TotalContexts.AddTag(GameplayTagSurfaceContext);
				}
			}
		}
	}
	for (UAudioComponent* ActiveAudioComponent : ActiveAudioComponents)
	{
		if (ActiveAudioComponent)
		{
			AudioComponentsToBeAdded.Add(ActiveAudioComponent);
		}
	}
	for (UNiagaraComponent* ActiveNiagaraComponent : ActiveNiagaraComponents)
	{
		if (ActiveNiagaraComponent)
		{
			NiagaraComponentsToBeAdded.Add(ActiveNiagaraComponent);
		}
	}

	if (const UWorld* World = GetWorld())
	{
		if (UWarContextEffectsSubsystem* WarContextEffectsSubsystem = World->GetSubsystem<UWarContextEffectsSubsystem>())
		{
			TArray<UAudioComponent*> AudioComponents;
			TArray<UNiagaraComponent*> NiagaraComponents;
			WarContextEffectsSubsystem->SpawnContextEffects(GetOwner(), StaticMeshComponent, Bone, LocationOffset, RotationOffset, MotionEffect, TotalContexts, AudioComponents, NiagaraComponents, VisualEffectScale, AudioVolume, AudioPitch);
			AudioComponentsToBeAdded.Append(AudioComponents);
			NiagaraComponentsToBeAdded.Append(NiagaraComponents);
		}
	}
	ActiveAudioComponents.Empty();
	ActiveAudioComponents.Append(AudioComponentsToBeAdded);
	ActiveNiagaraComponents.Empty();
	ActiveNiagaraComponents.Append(NiagaraComponentsToBeAdded);
}

void UWarContextEffectComponent::UpdateEffectContexts(FGameplayTagContainer NewEffectContexts)
{
	CurrentContexts.Reset(NewEffectContexts.Num());
	CurrentContexts.AppendTags(NewEffectContexts);
}

void UWarContextEffectComponent::UpdateLibraries(TSet<TSoftObjectPtr<UWarContextEffectsLibrary>> NewContextEffectsLibraries)
{
	CurrentWarContextEffectsLibraries = NewContextEffectsLibraries;
	if (const UWorld* World = GetWorld())
	{
		if (UWarContextEffectsSubsystem* WarContextEffectsSubsystem = World->GetSubsystem<UWarContextEffectsSubsystem>())
		{
			WarContextEffectsSubsystem->LoadAndAddContextEffectsLibraries(GetOwner(), CurrentWarContextEffectsLibraries);
		}
	}
}

// Called when the game starts
void UWarContextEffectComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentContexts.AppendTags(DefaultEffectContexts);
	CurrentWarContextEffectsLibraries = DefaultWarContextEffectsLibraries;
	if (const UWorld* World = GetWorld())
	{
		if (UWarContextEffectsSubsystem* WarContextEffectsSubsystem = World->GetSubsystem<UWarContextEffectsSubsystem>())
		{
			WarContextEffectsSubsystem->LoadAndAddContextEffectsLibraries(GetOwner(), CurrentWarContextEffectsLibraries);
		}
	}
}

void UWarContextEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const UWorld* World = GetWorld())
	{
		if (UWarContextEffectsSubsystem* WarContextEffectsSubsystem = World->GetSubsystem<UWarContextEffectsSubsystem>())
		{
			WarContextEffectsSubsystem->UnloadAndRemoveContextEffectsLibraries(GetOwner());
		}
	}
	Super::EndPlay(EndPlayReason);
}
