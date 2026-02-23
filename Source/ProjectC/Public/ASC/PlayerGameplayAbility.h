// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASC/BaseGameplayAbility.h"
#include "ProjectCEnum.h"
#include "PlayerGameplayAbility.generated.h"

class AProjectPlayerCharacter;


/**
 * 
 */
UCLASS()
class PROJECTC_API UPlayerGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

	UFUNCTION(BlueprintPure)
	AProjectPlayerCharacter* GetPlayerCharacterFromActorInfo();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dodge")
	EDodgeDirection DodgeDir = EDodgeDirection::Forward;

	UFUNCTION(BlueprintCallable)
	void InputModeUIOnly();

public: // OverlapDamage

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "Skill")
	TArray<AActor*> TargetActorArray;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UFUNCTION(BlueprintCallable)
	void OverlapDamage(AActor* TargetActor, float Damage);
};
