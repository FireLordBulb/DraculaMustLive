#pragma once
#include "CoreMinimal.h"
#include "ScytheState.h"
#include "GameFramework/Actor.h"
#include "Scythe.generated.h"

class UScytheHand;

UCLASS()
class DRACULAMUSTLIVE_API AScythe : public AActor
{
	GENERATED_BODY()

public:
	AScythe();

protected:
	virtual void BeginPlay() override;

public:
	void SetHand(USceneComponent* InHand);
	
	UFUNCTION(BlueprintCallable)
	void Throw(bool IsHoldActive);
	UFUNCTION(BlueprintCallable)
	void Recall(bool IsHoldActive);
	UFUNCTION(BlueprintCallable)
	void StartComboClick();
	UFUNCTION(BlueprintCallable)
	void MakeHeld();

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	EScytheState GetState() const
	{
		return StateChanger.Get();
	}
	
private:
	UFUNCTION(BlueprintCallable)
	void PawnOverlap(AActor* OtherActor, bool IsLargeHitbox);

	UFUNCTION(BlueprintCallable)
	void StaticOverlap(AActor* OtherActor);

	void TickRotation(float DeltaTime);

	void StartMoving(bool DoRotateClockwise);

	UPROPERTY(EditDefaultsOnly)
	float BaseDamage = 10;
	UPROPERTY(EditDefaultsOnly)
	float RecallDamage = 10;

	UPROPERTY(EditDefaultsOnly)
	float FlySpeed = 1000;
	UPROPERTY(EditDefaultsOnly)
	float SpinSpeed = -1000;
	UPROPERTY(EditDefaultsOnly)
	float ThrowRollAngle = 90;

	UPROPERTY()
	USceneComponent* Hand = nullptr;
	UPROPERTY()
	ACharacter* Reaper = nullptr;

	FVector ThrowDirection;
	
	int8 RotationSign = +1;
	
	struct FStateChanger
	{
		private:
			EScytheState State = EScytheState::Held;
		public:
			void Set(EScytheState InState, AScythe* Scythe);
			EScytheState Get() const
			{
				return State;
			}
	};
	FStateChanger StateChanger;
};
