// Personal Copyright
#pragma once
#include "GameFramework/Character.h"
#include "WarCharacter.generated.h"

class AWarInteractiveActor;
class AWarPlayerController;
class AWarPlayerState;
class AWarWeapon;
class UWarCameraComponent;
class UWarCameraMode;
class UWarHealthComponent;
class UWarInventoryComponent;
class UWarPawnComponent;
struct FInputActionValue;
struct FWarPlayerMappableInputConfigPair;

UCLASS()
class WARFIRSTPERSON_API AWarCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWarCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "War|Character")
	AWarPlayerController* GetWarPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "War|Character")
	AWarPlayerState* GetWarPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "War|Character")
	bool IsArmed();

	void SetIsArmed(bool Armed);
	bool AddOverlappedActor(AActor* Actor);
	bool RemoveOverlappedActor(AActor* Actor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& InputActionValue);
	void Turn(const FInputActionValue& InputActionValue);
	virtual void Jump() override;
	void ToggleCrouch();
	void Interact();
	void Fire();
	void AimDownSight();
	void EndAimDownSight();
	void CycleActiveWeaponForward();
	void CycleActiveWeaponBackward();
#if UE_BUILD_DEVELOPMENT
	void ToggleDebugWeidget();
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "War|Character")
	bool bIsArmed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "War|Character")
	bool bIsFiring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "War|Character")
	bool bIsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "War|Character")
	bool bIsAimingDownSight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "War|Character")
	bool bIsMelee;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "War|Character")
	TArray<FWarPlayerMappableInputConfigPair> DefaultInputConfigs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "War|Character")
	TObjectPtr<UWarHealthComponent> WarHealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "War|Character")
	TObjectPtr<UWarInventoryComponent> WarInventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "War|Character")
	TObjectPtr<UWarPawnComponent> WarPawnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWarCameraComponent> WarCameraComponent;

private:
	TArray<TObjectPtr<AWarInteractiveActor>> OverlappedActors;
	TObjectPtr<AWarInteractiveActor> HighestPriorityActor;
};
