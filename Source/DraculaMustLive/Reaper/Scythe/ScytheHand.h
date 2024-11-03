#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ScytheHand.generated.h"

class AScythe;

UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
class DRACULAMUSTLIVE_API UScytheHand : public USceneComponent
{
	GENERATED_BODY()

public:
	UScytheHand();

	UFUNCTION(BlueprintCallable)
	AScythe* GetScythe() const
	{
		return Scythe;
	}
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void OnInputStarted();
	UFUNCTION(BlueprintCallable)
	void OnInputCompleted();
	UFUNCTION(BlueprintCallable)
	void OnInputOngoing(const float ElapsedTime);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AScythe> ScytheClass;

	UPROPERTY()
	AScythe* Scythe = nullptr;

	bool bHoldIsActive = false;
};