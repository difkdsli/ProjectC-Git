// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectCAbilitySystemComponent.generated.h"

/**
 *
 */
UCLASS()
class PROJECTC_API UProjectCAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	bool bHealthCheck();

public:
	UPROPERTY(BlueprintReadWrite)
	float PreviousDamageStack{ 0 };

	UPROPERTY(BlueprintReadWrite)
	float DamageStack{ 0 };

	void SetDamageStack();
	void SetDamageStackTimer(float InDamageStack);
};
