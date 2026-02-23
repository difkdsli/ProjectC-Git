// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASC/EnemyGameplayAbility.h"
#include "WideSlashAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTC_API UWideSlashAbility : public UEnemyGameplayAbility
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void WideSlashOverlap(AActor* TargetActor);

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float WideSlashDamage{ 20 };

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "Skill")
	TArray<AActor*> TargetActorArray;
};
