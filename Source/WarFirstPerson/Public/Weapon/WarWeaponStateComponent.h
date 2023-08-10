// Personal Copyright
#pragma once
#include "Components/GameFrameworkComponent.h"
#include "WarWeaponStateComponent.generated.h"

UENUM(BlueprintType)
enum class EWarWeaponState : uint8
{
	Ready,
	Unequipped,
	NoOwner,
	Firing,
	Reloading,
	OutOfAmmunition,
	InvalidState
};

UCLASS()
class WARFIRSTPERSON_API UWarWeaponStateComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWarWeaponStateComponent(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCurrentWarWeaponState(const EWarWeaponState& NewWarWeaponState);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon State")
	EWarWeaponState GetCurrentWarWeaponState() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, BlueprintGetter = GetCurrentWarWeaponState, Category = "War|Weapon State")
	EWarWeaponState CurrentWarWeaponState;
};
