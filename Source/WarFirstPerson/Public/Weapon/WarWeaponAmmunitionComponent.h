// Personal Copyright
#pragma once
#include "Components/GameFrameworkComponent.h"
#include "WarWeaponAmmunitionComponent.generated.h"

UCLASS()
class WARFIRSTPERSON_API UWarWeaponAmmunitionComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWarWeaponAmmunitionComponent(const FObjectInitializer& ObjectInitializer);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCurrentMagazineCapacity(const int32& NewCurrentMagazineCapacity);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Ammunition")
	int32 GetCurrentMagazineCapacity() const;

	void SetCurrentAmmunitionCapacity(const int32& NewCurrentAmmunitionCapacity);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Ammunition")
	int32 GetCurrentAmmunitionCapacity() const;

	void SetMaxMagazineCapacity(const int32& NewMaxMagazineCapacity);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Ammunition")
	int32 GetMaxMagazineCapacity() const;

	void SetMaxAmmunitionCapacity(const int32& NewMaxAmmunitionCapacity);

	UFUNCTION(BlueprintCallable, Category = "War|Weapon Ammunition")
	int32 GetMaxAmmunitionCapacity() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "War|Weapon Ammunition Properties", BlueprintGetter =	GetCurrentMagazineCapacity)
	int32 CurrentMagazineCapacity = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "War|Weapon Ammunition Properties", BlueprintGetter = GetCurrentAmmunitionCapacity)
	int32 CurrentAmmunitionCapacity = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "War|Weapon Ammunition Properties", BlueprintGetter = GetMaxMagazineCapacity)
	int32 MaxMagazineCapacity = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "War|Weapon Ammunition Properties", BlueprintGetter = GetMaxAmmunitionCapacity)
	int32 MaxAmmunitionCapacity = 0;
};
