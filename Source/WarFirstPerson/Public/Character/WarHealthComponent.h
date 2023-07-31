// Personal Copyright
#pragma once
#include "Components/ActorComponent.h"
#include "WarHealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARFIRSTPERSON_API UWarHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWarHealthComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "War|Health")
	void SetCurrentHealth(int32 NewCurrentHealth);

	UFUNCTION(BlueprintCallable, Category = "War|Health")
	int32 GetCurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "War|Health")
	void SetMaxHealth(int32 NewMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "War|Health")
	int32 GetMaxHealth();

	UFUNCTION(BlueprintImplementableEvent, Category = "War|Health")
	void OnCurrentHealthLessThanOne();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "War|Health", Meta = (AllowPrivateAccess = true))
	int32 CurrentHealth;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "War|Health", Meta = (AllowPrivateAccess = true))
	int32 MaxHealth;
};
