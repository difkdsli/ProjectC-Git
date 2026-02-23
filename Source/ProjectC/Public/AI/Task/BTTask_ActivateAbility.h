// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "BTTask_ActivateAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTC_API UBTTask_ActivateAbility : public UBTTaskNode
{

	GENERATED_BODY()

public:

	UBTTask_ActivateAbility();

	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayTag AbilityTag;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
