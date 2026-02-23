// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ProjectC_GEEC.generated.h"

/**
 *
 */
UCLASS()
class PROJECTC_API UProjectC_GEEC : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UProjectC_GEEC();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
