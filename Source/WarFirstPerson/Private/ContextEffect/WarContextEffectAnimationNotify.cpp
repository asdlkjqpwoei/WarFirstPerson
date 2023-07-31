// Personal Copyright
#include "ContextEffect/WarContextEffectAnimationNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "ContextEffect/WarContextEffectComponent.h"
#include "ContextEffect/WarContextEffectsLibrary.h"
#include "ContextEffect/WarContextEffectsSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(WarContextEffectAnimationNotify)

UWarContextEffectAnimationNotify::UWarContextEffectAnimationNotify()
{

}

void UWarContextEffectAnimationNotify::PostLoad()
{
	Super::PostLoad();
}

#if WITH_EDITOR
void UWarContextEffectAnimationNotify::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

FString UWarContextEffectAnimationNotify::GetNotifyName_Implementation() const
{
	if (Effect.IsValid())
	{
		return Effect.ToString();
	}
	return Super::GetNotifyName_Implementation();
}

void UWarContextEffectAnimationNotify::Notify(USkeletalMeshComponent* SkeletalMeshComponent, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(SkeletalMeshComponent, Animation, EventReference);
	if (SkeletalMeshComponent)
	{
		if (AActor* OwningActor = SkeletalMeshComponent->GetOwner())
		{
			bool bHitSuccess = false;
			FHitResult HitResult;
			FCollisionQueryParams QueryParameters;
			if (WarTraceProperties.bIgnoreActor)
			{
				QueryParameters.AddIgnoredActor(OwningActor);
			}
			QueryParameters.bReturnPhysicalMaterial = true;
			if (bPerformTrace)
			{
				FVector TraceStart = bAttached ? SkeletalMeshComponent->GetSocketLocation(SocketName) : SkeletalMeshComponent->GetComponentLocation();
				if (UWorld* World = OwningActor->GetWorld())
				{
					bHitSuccess = World->LineTraceSingleByChannel(HitResult, TraceStart, (TraceStart + WarTraceProperties.EndTraceLocationOffset), WarTraceProperties.TraceChannel, QueryParameters, FCollisionResponseParams::DefaultResponseParam);
				}
			}
			FGameplayTagContainer Contexts;
			TArray<UObject*> WarContextEffectImplementingObject;
			TArray<UWarContextEffectComponent*> WarContextEffectComponents;
			for (UActorComponent* ActorComponent : OwningActor->GetComponents())
			{
				if (ActorComponent)
				{
					if (UWarContextEffectComponent* WarContextEffectComponent = Cast<UWarContextEffectComponent>(ActorComponent))
					{
						WarContextEffectComponents.Add(WarContextEffectComponent);
					}
				}
			}
			for (UWarContextEffectComponent* WarContextEffectComponent : WarContextEffectComponents)
			{
				if (WarContextEffectComponent)
				{
					WarContextEffectComponent->AnimMotionEffect((bAttached ? SocketName : FName("None")), Effect, SkeletalMeshComponent, LocationOffset, RotationOffset, Animation, bHitSuccess, HitResult, Contexts, WarVisualEffectProperties.Scale, WarAudioProperties.VolumeMultiplier, WarAudioProperties.PitchMultiplier);
				}
			}
#if WITH_EDITORONLY_DATA
			if (bPreviewInEditor)
			{
				UWorld* World = OwningActor->GetWorld();
				if (World && World->WorldType == EWorldType::EditorPreview)
				{
					Contexts.AppendTags(WarPreviewProperties.PreviewContexts);
					if (WarPreviewProperties.bPreviewPhysicalSurfaceAsContext)
					{
						TEnumAsByte<EPhysicalSurface> PhysicalSurfaceType = WarPreviewProperties.PreviewPhysicalSurface;
						if (const UWarContextEffectsSettings* WarContextEffectSettings = GetDefault<UWarContextEffectsSettings>())
						{
							if (const FGameplayTag* SurfaceContextPtr = WarContextEffectSettings->SurfaceTypeToContextMap.Find(PhysicalSurfaceType))
							{
								FGameplayTag SurfaceContext = *SurfaceContextPtr;
								Contexts.AddTag(SurfaceContext);
							}
						}
					}
					if (UObject* EffectsLibrariesObject = WarPreviewProperties.PreviewContextEffectsLibrary.TryLoad())
					{
						if (UWarContextEffectsLibrary* WarContextEffectsLibrary = Cast<UWarContextEffectsLibrary>(EffectsLibrariesObject))
						{
							TArray<USoundBase*> TotalSounds;
							TArray<UNiagaraSystem*> TotalNiagaraSystems;
							WarContextEffectsLibrary->LoadEffects();
							if (WarContextEffectsLibrary && WarContextEffectsLibrary->GetContextEffectsLibraryLoadState() == EWarContextEffectsLibraryLoadState::Loaded)
							{
								TArray<USoundBase*> Sounds;
								TArray<UNiagaraSystem*> NiagaraSystems;
								WarContextEffectsLibrary->GetEffects(Effect, Contexts, Sounds, NiagaraSystems);
								TotalSounds.Append(Sounds);
								TotalNiagaraSystems.Append(NiagaraSystems);
							}
							for (USoundBase* Sound : TotalSounds)
							{
								UGameplayStatics::SpawnSoundAttached(Sound, SkeletalMeshComponent, (bAttached ? SocketName : FName("None")), LocationOffset, RotationOffset, EAttachLocation::KeepRelativeOffset, false, WarAudioProperties.VolumeMultiplier, WarAudioProperties.PitchMultiplier, 0.0f, nullptr, nullptr, true);
							}
							for (UNiagaraSystem* NiagaraSystem : TotalNiagaraSystems)
							{
								UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, SkeletalMeshComponent, (bAttached ? SocketName : FName("None")), LocationOffset, RotationOffset, WarVisualEffectProperties.Scale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true, true);
							}
						}
					}
				}
			}
#endif
		}
	}
}

#if WITH_EDITOR
void UWarContextEffectAnimationNotify::ValidateAssociatedAssets()
{
	Super::ValidateAssociatedAssets();
}

void UWarContextEffectAnimationNotify::SetParameters(FGameplayTag EffectIn, FVector LocationOffsetIn, FRotator RotationOffsetIn, FWarContextEffectAnimationNotifyVisualEffectSettings WarVisualEffectPropertiesIn, FWarContextEffectAnimationNotifyAudioSettings WarAudioPropertiesIn, bool bAttachedIn, FName SocketNameIn, bool bPerformTraceIn, FWarContextEffectAnimationNotifyTraceSettings WarTracePropertiesIn)
{
	Effect = EffectIn;
	LocationOffset = LocationOffsetIn;
	RotationOffset = RotationOffsetIn;
	WarVisualEffectProperties.Scale = WarVisualEffectPropertiesIn.Scale;
	WarAudioProperties.PitchMultiplier = WarAudioPropertiesIn.PitchMultiplier;
	WarAudioProperties.VolumeMultiplier = WarAudioPropertiesIn.VolumeMultiplier;
	bAttached = bAttachedIn;
	SocketName = SocketNameIn;
	bPerformTrace = bPerformTraceIn;
	WarTraceProperties.EndTraceLocationOffset = WarTracePropertiesIn.EndTraceLocationOffset;
	WarTraceProperties.TraceChannel = WarTracePropertiesIn.TraceChannel;
	WarTraceProperties.bIgnoreActor = WarTracePropertiesIn.bIgnoreActor;
}
#endif

