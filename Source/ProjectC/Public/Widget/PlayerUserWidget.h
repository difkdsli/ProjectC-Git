// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseUserWidget.h"
#include "PlayerUserWidget.generated.h"

/**
 *
 */
class AProjectPlayerCharacter;
class UProjectCAttributeSet;

UCLASS()
class PROJECTC_API UPlayerUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly)
	AProjectPlayerCharacter* OwningPlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
	APlayerController* OwningPlayerController;

public: // AttributeSet

	UPROPERTY(BlueprintReadOnly)
	UProjectCAttributeSet* OwningAttributeSet;

	UPROPERTY(BlueprintReadOnly)
	float MaxStamina{ 7 };

	UPROPERTY(BlueprintReadOnly)
	float CurrentStamina{ 7 };

	UPROPERTY(BlueprintReadOnly)
	float MaxMana{ 7 };

	UPROPERTY(BlueprintReadOnly)
	float CurrentMana{ 7 };

	UFUNCTION(BlueprintCallable)
	void IncreaseAttackPowerValue(float RuneCost);

	UFUNCTION(BlueprintCallable)
	void IncreaseMagicPowerValue(float RuneCost);

	UFUNCTION(BlueprintCallable)
	void IncreaseHealthValue(float RuneCost);

	UFUNCTION(BlueprintCallable)
	void IncreaseManaValue(float RuneCost);

	UFUNCTION(BlueprintCallable)
	void IncreaseStaminaValue(float RuneCost);

	UFUNCTION(BlueprintCallable)
	void IncreaseLevelValue();

	void ApplyLevelUpCost(float RuneCost);

	UFUNCTION(BlueprintCallable)
	void HealthFlaskUp(int& HealthPotion, int& ManaPotion);

	UFUNCTION(BlueprintCallable)
	void ManaFlaskUp(int& HealthPotion, int& ManaPotion);

	UFUNCTION(BlueprintCallable)
	void CurrentFlaskUp(int& HealthPotion, int& ManaPotion);

	UFUNCTION(BlueprintCallable)
	void DisableInput();

	UFUNCTION(BlueprintCallable)
	void EnableInput();
	
	// UI
public:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayBloodAnim();
};
