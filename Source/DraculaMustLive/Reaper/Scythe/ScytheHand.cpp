#include "ScytheHand.h"

#include "Scythe.h"

UScytheHand::UScytheHand()
{
}

void UScytheHand::BeginPlay()
{
	Scythe = GetWorld()->SpawnActor<AScythe>(ScytheClass, GetComponentTransform());
	Scythe->AttachToComponent(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	Super::BeginPlay();
}

void UScytheHand::OnInputStarted()
{
	// TODO: Forward to comboClick start
	Scythe = &*Scythe;
}

void UScytheHand::OnInputCompleted()
{
	// TODO: Forward to throw/recall start
	Scythe = &*Scythe;
}

void UScytheHand::OnInputOngoing(const float ElapsedTime)
{
	// TODO: implement bHoldIsActive setting
	bHoldIsActive = !!bHoldIsActive;
}
