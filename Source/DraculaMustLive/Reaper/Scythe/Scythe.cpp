#include "Scythe.h"

#include "DraculaMustLive/Health/Health.h"
// ReSharper disable once CppUnusedIncludeDirective // This isn't unused, it's needed for pointer equality checks of Reaper.
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

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
	EarliestRecallTime = MinRecallDelay+GetWorld()->GetTimeSeconds();
	
	auto PlayerCameraManager = GetWorld()->GetPlayerControllerIterator()->Get()->PlayerCameraManager;
	FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
	FVector CameraRotation = PlayerCameraManager->GetCameraRotation().RotateVector(FVector::ForwardVector);
	FVector TraceEnd = CameraLocation + VeryLongDistance*CameraRotation;
	
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, FCollisionQueryParams(FName(),  false, Reaper));

	FRotator RotationTowardCrosshairTarget = UKismetMathLibrary::FindLookAtRotation(CameraLocation, HitResult.bBlockingHit ? HitResult.Location : TraceEnd);
	RotationTowardCrosshairTarget.Roll = ThrowRollAngle;
	SetActorRotation(RotationTowardCrosshairTarget);
	ThrowDirection = GetActorForwardVector();
}

void AScythe::Recall(bool IsHoldActive)
{
	if (EarliestRecallTime < GetWorld()->GetTimeSeconds())
	{
		StateChanger.Set(EScytheState::Recalled, this);
	}
}

void AScythe::StartMouseClick()
{
	// TODO: Check timing window and also set PiercingUpgrade.DidClickTooEarly accordingly.
	PiercingUpgrade.State = EPiercingState::MouseDown;
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
	if (Reaper == OtherActor || PiercedEnemies.Contains(OtherActor))
	{
		return;
	}
	auto Health = OtherActor->GetComponentByClass<UHealth>();
	if (!Health)
	{
		return;
	}
	bool IsRecalled =  StateChanger.Get() == EScytheState::Recalled;
	float Damage = IsRecalled ? RecallDamage : BaseDamage;
	if (DidActorSurviveDamage(Health, Damage))
	{
		FVector Difference = OtherActor->GetActorLocation()-GetActorLocation();
		float GrazeDotProduct = ThrowDirection.Dot(Difference.GetUnsafeNormal());
		if (IsRecalled || PiercingUpgrade.State == EPiercingState::Active || GrazeDotProduct <  GrazeMaxDotProduct)
		{
			PiercedEnemies.Add(OtherActor);
		} else
		{
			AttachToActor(OtherActor, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
			StateChanger.Set(EScytheState::Stuck, this);
		}
	}
}

void AScythe::StaticOverlap(AActor* OtherActor)
{
	if (StateChanger.Get() == EScytheState::Thrown)
	{
		StateChanger.Set(EScytheState::Stuck, this);
	}
}

bool AScythe::DidActorSurviveDamage(UHealth* Health, float Damage)
{
	return Health->TakeDamage(Damage);
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
		Scythe->PiercedEnemies.Empty();
		break;
	case EScytheState::Thrown:
		Scythe->SetActorEnableCollision(true);
		Scythe->StartMoving(true);
		break;
	case EScytheState::Stuck:
		Scythe->SetActorEnableCollision(false);
		Scythe->PiercedEnemies.Empty();
		break;
	case EScytheState::Recalled:
		Scythe->PiercedEnemies.Empty();
		if (auto* AttachedParent = Scythe->RootComponent->GetAttachParent())
		{
			Scythe->PiercedEnemies.Add(AttachedParent->GetOwner());
		}
		Scythe->SetActorEnableCollision(true);
		Scythe->StartMoving(false);
		break;
	}
	State = InState;
}
