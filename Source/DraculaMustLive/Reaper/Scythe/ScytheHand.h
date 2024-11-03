#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScytheHand.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DRACULAMUSTLIVE_API UScytheHand : public UActorComponent
{
	GENERATED_BODY()

public:
	UScytheHand();

protected:
	virtual void BeginPlay() override;

public:
	
};
