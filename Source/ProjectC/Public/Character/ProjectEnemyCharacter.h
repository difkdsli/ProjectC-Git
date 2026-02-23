// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ProjectBaseCharacter.h"
#include "ProjectEnemyCharacter.generated.h"

class UWidgetComponent;
class UEnemyUserWidget;
class AEnemyAIController;

/**
 * 
 */
UCLASS()
class PROJECTC_API AProjectEnemyCharacter : public AProjectBaseCharacter
{
	GENERATED_BODY()
	
public:
	AProjectEnemyCharacter();

protected:

	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UWidgetComponent* TargetLockOnWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetComponent* HPWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeSinceLastHit = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HealthBarTimeout = 5.f;

	bool bHealthBarTimeOut = true;

public: // AI

	AEnemyAIController* EnemyAIController;

	void SetTargetActor(UObject* Target);

	void CheckTargetActor();

public: // AttributeSet

	virtual void OnAnyAttributeChanged(const FOnAttributeChangeData& Data)override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	bool bIsBoss = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UBaseUserWidget> HPWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UBaseUserWidget* HPUserWidget;

};
