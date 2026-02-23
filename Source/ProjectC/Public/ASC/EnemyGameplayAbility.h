// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASC/BaseGameplayAbility.h"
#include "EnemyGameplayAbility.generated.h"


class AProjectEnemyCharacter;

/**
 * 
 */
UCLASS()
class PROJECTC_API UEnemyGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure)
	AProjectEnemyCharacter* GetEnemyCharacterFromActorInfo();

	
};
