// Personal Copyright
#pragma once
#include "GameFramework/Actor.h"
#include "WarInteractiveActor.generated.h"

UCLASS()
class WARFIRSTPERSON_API AWarInteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarInteractiveActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(APawn* Pawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
