// Personal Copyright
#pragma once
#include "InteractiveActor/WarInteractiveActor.h"
#include "GameFramework/Actor.h"
#include "WarWeapon.generated.h"

struct FHitResult;
struct FGameplayTag;
class AWarWeaponProjectile;
class UNiagaraSystem;
class UPrimitiveComponent;
class USphereComponent;
class UWarInventoryItemDefinition;
class UWarWeaponAmmunitionComponent;
class UWarWeaponPropertiesComponent;
class UWarWeaponStateComponent;

struct FWarWeaponFireParameters
{
	FVector TraceStart;
	FVector AimEnd;
	FVector AimDirection;
	FWarWeaponFireParameters() : TraceStart(ForceInitToZero), AimEnd(ForceInitToZero), AimDirection(ForceInitToZero)
	{

	}
};

UCLASS()
class WARFIRSTPERSON_API AWarWeapon : public AWarInteractiveActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarWeapon();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(APawn* Pawn) override;
	void Fire();
	void Reload();

	UFUNCTION(BlueprintImplementableEvent, Category = "War|Weapon")
	void OnFiring();

	UFUNCTION(BlueprintImplementableEvent, Category = "War|Weapon")
	void OnReloading();

	UFUNCTION(BlueprintCallable, Category = "War|Weapon")
	void OnFired();

	UFUNCTION(BlueprintCallable, Category = "War|Weapon")
	void OnReloaded();

	UFUNCTION(BlueprintCallable, Category = "War|Weapon")
	bool PickupWeapon(APawn* PickingPawn);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon")
	bool DropWeapon(APawn* DropingPawn);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "War|Weapon")
	TSubclassOf<UWarInventoryItemDefinition> ItemDefinition;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Targeting(OUT TArray<FHitResult>& OutHitResults);
	FTransform GetTargetingTransform(APawn* Pawn) const;
	FVector GetWeaponTargetingLocation() const;
	void TraceBulletsInShot(const FWarWeaponFireParameters& WeaponFireParameters, OUT TArray<FHitResult>& OutHitResult);
	FVector VRandomConeNormalDistribution(const FVector& Direction, const float ConeHalfAngleRad, const float Exponent);
	FHitResult TraceSingleBullet(const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius, bool bIsSimulated, OUT TArray<FHitResult>& OutHitResults) const;
	int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults) const;
	FHitResult WeaponTrace(const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const;
	void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& CollisionQueryParameters) const;
	void AddWeaponSpread();
	float ClampHeat(float NewHeat);
	void ComputeHeatRange(float& MinHeat, float& MaxHeat);
	bool UpdateSpread(float DeltaTime);
	bool UpdateMultipliers(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, Category = "War|Weapon")
	void OnWeaponPickingUp();
	
	UFUNCTION(BlueprintCallable, Category = "War|Weapon Physics")
	void OnCollisionBeginSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Physics")
	void OnCollisionEndSphereOverplay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<AWarWeaponProjectile> Bullet;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UWarWeaponPropertiesComponent> WarWeaponPropertiesComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UWarWeaponAmmunitionComponent> WarWeaponAmmunitionComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UWarWeaponStateComponent> WarWeaponStateComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Animation")
	FName AttachSkeletalSocket;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Animation")
	FTransform WeaponAttachTransform;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Mesh")
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Mesh")
	TObjectPtr<UStaticMesh> ShellMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Effect")
	TObjectPtr<UNiagaraSystem> ShellEjectEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Physics")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Animation")
	TSubclassOf<UAnimInstance> AnimationLayer;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Effect")
	TObjectPtr<UNiagaraSystem> MuzzleFlash;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Weapon Effect")
	TObjectPtr<UNiagaraSystem> Tracer;

private:
	bool bIsReloading;
	bool bIsFiring;

	double LastFireTime;
};
