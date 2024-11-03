#include "ScytheHand.h"
#include "Scythe.h"
#include "Kismet/GameplayStatics.h"

UScytheHand::UScytheHand()
{
}

void UScytheHand::BeginPlay()
{
	Scythe = Cast<AScythe>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ScytheClass, GetComponentTransform()));
	Scythe->SetHand(this);
	UGameplayStatics::FinishSpawningActor(Scythe, GetComponentTransform());
	Super::BeginPlay();
}

void UScytheHand::OnInputStarted() const
{
	Scythe->StartComboClick();
}

void UScytheHand::OnInputCompleted() const
{
	switch(Scythe->GetState())
	{
	case EScytheState::Held:
	case EScytheState::Recalled:
		Scythe->Throw(IsHoldActive);
		break;
	case EScytheState::Thrown:
	case EScytheState::Stuck:
		Scythe->Recall(IsHoldActive);	
		break;
	}
}

void UScytheHand::OnInputOngoing(const float ElapsedSeconds)
{
	// TODO: implement bHoldIsActive setting
	IsHoldActive = !!IsHoldActive;
}
