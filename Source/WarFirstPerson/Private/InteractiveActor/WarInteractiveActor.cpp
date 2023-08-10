// Personal Copyright
#include "InteractiveActor/WarInteractiveActor.h"

// Sets default values
AWarInteractiveActor::AWarInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called every frame
void AWarInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarInteractiveActor::Interact(APawn* Pawn)
{

}

// Called when the game starts or when spawned
void AWarInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}
