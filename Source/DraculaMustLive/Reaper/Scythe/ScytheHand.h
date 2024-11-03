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

protected:
	virtual void BeginPlay() override;

public: 
	UFUNCTION(BlueprintCallable)
	AScythe* GetScythe() const
	{
		return Scythe;
	}
	
private:
	UFUNCTION(BlueprintCallable)
	void OnInputStarted() const;
	UFUNCTION(BlueprintCallable)
	void OnInputCompleted() const;
	UFUNCTION(BlueprintCallable)
	void OnInputOngoing(const float ElapsedSeconds);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AScythe> ScytheClass;

	UPROPERTY()
	AScythe* Scythe = nullptr;

	bool IsHoldActive = false;
};
