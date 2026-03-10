// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASC/EnemyGameplayAbility.h"
#include "Grux_Brust.generated.h"
class AEnemyProjectile;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class PROJECTC_API UGrux_Brust : public UEnemyGameplayAbility
{
	GENERATED_BODY()

public:
	UGrux_Brust();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;
	
	UPROPERTY(EditDefaultsOnly)
	float TestZ=100;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyProjectile> MagmaClass;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* SkillAnimMontage;
	
	UFUNCTION()
	void OnMontageFinished();
};
