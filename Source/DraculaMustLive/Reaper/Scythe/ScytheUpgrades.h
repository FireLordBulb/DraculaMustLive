#pragma once
#include "ScytheUpgrades.generated.h"

enum class EPiercingState : uint8
{
	Inactive,
	ClickedTooEarly,
	MouseDown,
	Active
};

USTRUCT(BlueprintType)
struct DRACULAMUSTLIVE_API FPiercingAttack
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool IsUnlocked;
	UPROPERTY(BlueprintReadWrite)
	bool IsManaStealUnlocked;
	UPROPERTY(BlueprintReadWrite)
	bool IsSpeedMultiplierUnlocked;

	UPROPERTY(EditDefaultsOnly)
	float DamageMultiplier = 1;
	UPROPERTY(EditDefaultsOnly)
	float ActivationDistance = 100;
	UPROPERTY(EditDefaultsOnly)
	float PostCatchTimeWindow = 0.1f;
	UPROPERTY(EditDefaultsOnly)
	float ManaPerDamage = 0.1f;
	UPROPERTY(EditDefaultsOnly)
	float SpeedMultiplier = 2;

	EPiercingState State;
	float LatestActivationTime;
	float Damage;
};

USTRUCT(BlueprintType)
struct DRACULAMUSTLIVE_API FDashAttack
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool IsUnlocked;
	UPROPERTY(BlueprintReadWrite)
	bool IsSpeedBoostUnlocked;
	UPROPERTY(BlueprintReadWrite)
	bool IsHealthStealUnlocked;

	UPROPERTY(EditDefaultsOnly)
	float Speed = 1000;
	UPROPERTY(EditDefaultsOnly)
	float CatchRadius = 100;
	UPROPERTY(EditDefaultsOnly)
	float Damage = 50;
	UPROPERTY(EditDefaultsOnly)
	float SpeedBoostMultiplier = 2;
	UPROPERTY(EditDefaultsOnly)
	float SpeedBoostDuration = 5;
	UPROPERTY(EditDefaultsOnly)
	float HealthPerDamage = 0.1f;
	UPROPERTY(EditDefaultsOnly)
	FName DashingCollisionProfile;
	
	FName DefaultCollisionProfile;
	bool IsActive;
	float DefaultSpeed;
};

USTRUCT(BlueprintType)
struct DRACULAMUSTLIVE_API FChargedAttack
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool IsUnlocked;
	UPROPERTY(BlueprintReadWrite)
	bool IsDurationIncreaseUnlocked;
	UPROPERTY(BlueprintReadWrite)
	bool IsRangeIncreaseUnlocked;

	UPROPERTY(EditDefaultsOnly, meta = (UIMin = 20000, UIMax = 200000))
	float Impulse = 100000;
	UPROPERTY(EditDefaultsOnly)
	float LevitationDuration = 1;
	UPROPERTY(EditDefaultsOnly)
	float StunDuration = 3;
	UPROPERTY(EditDefaultsOnly)
	float IncreasedLevitationDuration = 2;

	bool IsActive;
};