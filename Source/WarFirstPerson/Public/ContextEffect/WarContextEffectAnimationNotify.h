// Personal Copyright
#pragma once
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WarContextEffectAnimationNotify.generated.h"

USTRUCT(BlueprintType)
struct WARFIRSTPERSON_API FWarContextEffectAnimationNotifyVisualEffectSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	FVector Scale = FVector(1.0f, 1.0f, 1.0f);
};

USTRUCT(BlueprintType)
struct WARFIRSTPERSON_API FWarContextEffectAnimationNotifyAudioSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	float PitchMultiplier = 1.0f;
};

USTRUCT(BlueprintType)
struct WARFIRSTPERSON_API FWarContextEffectAnimationNotifyTraceSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trace)
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trace)
	FVector EndTraceLocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trace)
	bool bIgnoreActor = true;
};

USTRUCT(BlueprintType)
struct WARFIRSTPERSON_API FWarContextEffectAnimationNotifyPreviewSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Preview)
	bool bPreviewPhysicalSurfaceAsContext = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Preview, meta = (EditCondition = "bPreviewPhysicalSurfaceAsContext"))
	TEnumAsByte<EPhysicalSurface> PreviewPhysicalSurface = EPhysicalSurface::SurfaceType_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Preview, meta = (AllowedClasses = "/Script/WarFirstPerson.WarContextsEffectLibrary"))
	FSoftObjectPath PreviewContextEffectsLibrary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Preview)
	FGameplayTagContainer PreviewContexts;
};

/**
 * 
 */
UCLASS(const, hidecategories = Object, CollapseCategories, Config = Game, meta = (DisplayName = "Play Context Effects"))
class WARFIRSTPERSON_API UWarContextEffectAnimationNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UWarContextEffectAnimationNotify();
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* SkeletalMeshComponent, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
#if WITH_EDITOR
	virtual void ValidateAssociatedAssets() override;

	UFUNCTION(BlueprintCallable)
	void SetParameters(FGameplayTag EffectIn, FVector LocationOffsetIn, FRotator RotationOffsetIn, FWarContextEffectAnimationNotifyVisualEffectSettings VisualEffectPropertiesIn, FWarContextEffectAnimationNotifyAudioSettings AudioPropertiesIn, bool bAttachedIn, FName SocketNameIn, bool bPerformTraceIn, FWarContextEffectAnimationNotifyTraceSettings TracePropertiesIn);
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "War|AnimationNotify", meta = (DisplayName = "Effect", ExposeOnSpawn = true))
	FGameplayTag Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "War|AnimationNotify", meta = (ExposeOnSpawn = true))
	FVector LocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "War|AnimationNotify", meta = (ExposeOnSpawn = true))
	FRotator RotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "War|AnimationNotify", meta = (ExposeOnSpawn = true))
	FWarContextEffectAnimationNotifyVisualEffectSettings WarVisualEffectProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "War|AnimationNotify", meta = (ExposeOnSpawn = true))
	FWarContextEffectAnimationNotifyAudioSettings WarAudioProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "War|AttachmentProperties", meta = (ExposeOnSpawn = true))
	uint32 bAttached : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "War|AttachmentProperties", meta = (ExposeOnSpanw = true, EditCondition = "bAttached"))
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "War|AnimationNotify", meta = (ExposeOnSpawn = true))
	uint32 bPerformTrace : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "War|AnimationNotify", meta = (ExposeOnSpawn = true))
	FWarContextEffectAnimationNotifyTraceSettings WarTraceProperties;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Config, EditAnywhere, Category = "War|PreviewProperties")
	uint32 bPreviewInEditor : 1;

	UPROPERTY(EditAnywhere, Category = "War|PreviewProperties", meta = (EditCondition = "bPreviewInEditor"))
	FWarContextEffectAnimationNotifyPreviewSettings WarPreviewProperties;
#endif
private:
};
