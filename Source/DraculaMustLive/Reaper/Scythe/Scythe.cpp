#include "Scythe.h"

#include "DraculaMustLive/Health/Health.h"
// ReSharper disable once CppUnusedIncludeDirective // This isn't unused, it's needed for pointer equality checks of Reaper.
#include "GameFramework/Character.h"

AScythe::AScythe()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AScythe::BeginPlay()
{
	MakeHeld();
	Reaper = GetWorld()->GetPlayerControllerIterator()->Get()->GetCharacter();
	Super::BeginPlay();
}

void AScythe::SetHand(USceneComponent* InHand)
{
	Hand = InHand;
}

void AScythe::Throw(bool IsHoldActive)
{
	if (StateChanger.Get() != EScytheState::Held)
	{
		// TODO: buffered combo input
		return;
	}
	StateChanger.Set(EScytheState::Thrown, this);
	// TODO: Replace with direction toward lineTrace from center of screen.
	ThrowDirection = GetActorForwardVector();
	FRotator Rotation = GetActorRotation();
	Rotation.Roll = ThrowRollAngle;
	SetActorRotation(Rotation);
}

void AScythe::Recall(bool IsHoldActive)
{
	StateChanger.Set(EScytheState::Recalled, this);
}

void AScythe::StartComboClick()
{
	// TODO
	FlySpeed = FlySpeed+1-1;
}

void AScythe::MakeHeld()
{
	StateChanger.Set(EScytheState::Held, this);
}

void AScythe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (StateChanger.Get())
	{
	case EScytheState::Held:
	case EScytheState::Stuck:
		break;
	case EScytheState::Thrown:
		AddActorWorldOffset(FlySpeed*DeltaTime*ThrowDirection);
		TickRotation(DeltaTime);
		break;
	case EScytheState::Recalled:
		float RecallAlpha = FMath::Pow(RecallPowerBase, DeltaTime);
		float LinearSpeedAlpha = 1-DeltaTime*FlySpeed/FVector::Distance(GetActorLocation(), Hand->GetComponentLocation());
		RecallAlpha = FMath::Min(RecallAlpha, LinearSpeedAlpha);
		if (RecallCloseEnoughAlpha < RecallAlpha)
		{
			SetActorLocation(FMath::Lerp(Hand->GetComponentLocation(), GetActorLocation(), RecallAlpha));
			TickRotation(DeltaTime);
		} else
		{
			MakeHeld();
		}
		break;
	}
}

void AScythe::PawnOverlap(AActor* OtherActor, bool IsLargeHitbox)
{
	EScytheState State = StateChanger.Get();
	if (!(State == EScytheState::Thrown || State == EScytheState::Recalled) || Reaper == OtherActor)
	{
		return;
	}
	auto Health = OtherActor->GetComponentByClass<UHealth>();
	if (!Health)
	{
		return;
	}
	bool DidSurvive = Health->TakeDamage(State == EScytheState::Recalled ? RecallDamage : BaseDamage);
	if (DidSurvive && State == EScytheState::Thrown)
	{
		AttachToActor(OtherActor, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
		StateChanger.Set(EScytheState::Stuck, this);
	}
}

void AScythe::StaticOverlap(AActor* OtherActor)
{
	if (StateChanger.Get() == EScytheState::Thrown)
	{
		StateChanger.Set(EScytheState::Stuck, this);
	}
}

void AScythe::TickRotation(const float DeltaTime)
{
	AddActorLocalRotation(FRotator(SpinSpeed*DeltaTime*RotationSign, 0, 0));
}

void AScythe::StartMoving(bool DoRotateClockwise)
{
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
	RotationSign = DoRotateClockwise ? +1 : -1;
}

void AScythe::FStateChanger::Set(const EScytheState InState, AScythe* Scythe)
{
	switch (InState)
	{
	case EScytheState::Held:
		Scythe->AttachToComponent(Scythe->Hand, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
		Scythe->SetActorEnableCollision(false);
		break;
	case EScytheState::Thrown:
		Scythe->SetActorEnableCollision(true);
		Scythe->StartMoving(true);
		break;
	case EScytheState::Stuck:
	
		break;
	case EScytheState::Recalled:
		Scythe->StartMoving(false);
		break;
	}
	State = InState;
}
