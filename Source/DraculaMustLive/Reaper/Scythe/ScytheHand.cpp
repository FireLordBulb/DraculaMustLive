#include "ScytheHand.h"
#include "Scythe.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h" 
#include "GameFramework/PlayerInput.h"
#include "EnhancedInputSubsystems.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

UScytheHand::UScytheHand()
{
}

void UScytheHand::BeginPlay()
{
 	for (auto& KeyMapping : MappingContext->GetMappings())
 	{
 		if (KeyMapping.Action != InputAction)
 		{
 			continue;
 		}
 		UInputTriggerHold* TriggerHold = nullptr;
 		KeyMapping.Triggers.FindItemByClass<UInputTriggerHold>(&TriggerHold);
 		if (TriggerHold != nullptr)
 		{
 			HoldTimeThreshold = TriggerHold->HoldTimeThreshold;
 		}
 		break;
 	}
	
	Scythe = Cast<AScythe>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ScytheClass, GetComponentTransform()));
	Scythe->SetHand(this);
	UGameplayStatics::FinishSpawningActor(Scythe, GetComponentTransform());
	Super::BeginPlay();
}

void UScytheHand::OnInputStarted() const
{
	switch(Scythe->GetState())
	{
	case EScytheState::Held:
	case EScytheState::Recalled:
		Scythe->StartMouseClick();
		break;
	case EScytheState::Thrown:
	case EScytheState::Stuck:
		break;
	}
	
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
	// Subtract one frame of time from the threshold to avoid overshooting.
	if (HoldTimeThreshold-FApp::GetTimecodeFrameRate().AsInterval() <= ElapsedSeconds)
	{
		IsHoldActive = true;
	}
}
