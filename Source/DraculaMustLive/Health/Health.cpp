// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"


// Sets default values for this component's properties
UHealth::UHealth()
{
	ResetToMax();
}


// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();
}

void UHealth::Heal(const float Amount)
{
	Current += Amount;
	if (Max < Current)
	{
		ResetToMax();
	}
}

bool UHealth::TakeDamage(const float Amount)
{
	Current -= Amount;
	UE_LOG(LogTemp, Log, TEXT("Health left: %f"), Current);
	if (Current < 0)
	{
		Current = 0;
		GetOwner()->Destroy();
		return false;
	}
	return true;
}

void UHealth::ResetToMax()
{
	Current = Max;
}
