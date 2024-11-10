#pragma once
#include "CoreMinimal.h"
#include "ScytheState.h"
#include "ScytheUpgrades.h"
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

	UPROPERTY(EditDefaultsOnly, meta = (Category = "Scythe|Damage"))
	float BaseDamage = 10;
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Scythe|Damage"))
	float RecallDamage = 10;

	UPROPERTY(EditDefaultsOnly, meta = (Category = "Scythe|Movement"))
	float FlySpeed = 1000;
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Scythe|Movement"))
	float SpinSpeed = -1000;
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Scythe|Movement", ClampMin = -180, ClampMax = 180))
	float ThrowRollAngle = 90;
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Scythe|Movement", ClampMin = 0, ClampMax = 1))
	float RecallPowerBase = 0.01f;
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Scythe|Movement", ClampMin = 0, ClampMax = 1))
	float RecallCloseEnoughAlpha = 0.1f;
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Scythe|Movement", ClampMin = -1, ClampMax = +1))
	float GrazeMaxDotProduct = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess, Category = "Scythe|Upgrades"))
	FPiercingAttack PiercingUpgrade = FPiercingAttack();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess, Category = "Scythe|Upgrades"))
	FDashAttack DashUpgrade = FDashAttack();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess, Category = "Scythe|Upgrades"))
	FChargedAttack ChargeUpgrade = FChargedAttack();
	
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

	static constexpr float VeryLongDistance = 10000;
};
