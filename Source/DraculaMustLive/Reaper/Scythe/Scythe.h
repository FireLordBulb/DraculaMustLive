#pragma once
#include "CoreMinimal.h"
#include "ScytheState.h"
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

	UFUNCTION(BlueprintCallable)
	EScytheState GetState() const
	{
		return StateChanger.Get();
	}
	
private:
	struct FStateChanger
	{
		private:
			EScytheState State = EScytheState::Held;
		public:
			void Set(const EScytheState InState, AScythe* Scythe);
			EScytheState Get() const
			{
				return State;
			}
	};
	FStateChanger StateChanger;
};
