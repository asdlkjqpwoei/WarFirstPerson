// Personal Copyright
#pragma once
#include "Components/ActorComponent.h"
#include "WarWeaponStateComponent.generated.h"

UENUM(BlueprintType)
enum class EWarWeaponState : uint8
{
	Ready,
	Firing,
	Reloading,
	OutOfAmmunition,
	Unequipped,
	InvalidState
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARFIRSTPERSON_API UWarWeaponStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWarWeaponStateComponent();

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
