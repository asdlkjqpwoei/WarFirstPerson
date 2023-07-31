// Personal Copyright
#pragma once
#include "GameFramework/Actor.h"
#include "WarWeaponProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UProjectilePropertiesComponent;

UCLASS()
class WARFIRSTPERSON_API AWarWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarWeaponProjectile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "War|Projectile Mesh")
	TObjectPtr<USkeletalMeshComponent> ProjectileMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "War|Projectile Physics")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "War|Projectile Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "War|Projectile Properties")
	TObjectPtr<UProjectilePropertiesComponent> ProjectilePropertiesComponent;
};
