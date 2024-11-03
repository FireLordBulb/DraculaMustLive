#include "Scythe.h"

AScythe::AScythe()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AScythe::BeginPlay()
{
	Super::BeginPlay();
	
}

void AScythe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
