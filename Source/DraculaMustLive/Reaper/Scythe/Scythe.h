#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Scythe.generated.h"

UCLASS()
class DRACULAMUSTLIVE_API AScythe : public AActor
{
	GENERATED_BODY()

public:
	AScythe();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
