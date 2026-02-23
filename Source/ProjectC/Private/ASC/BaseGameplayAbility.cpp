// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/BaseGameplayAbility.h"
#include "ASC/ProjectCGameplayTags.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "Character/ProjectBaseCharacter.h"
#include "Weapon/WeaponBaseActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DebugHelper.h"

void UBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 무기 공격 중복 오버랩 현상 막는 것 제거
	if (AProjectBaseCharacter* Character = Cast<AProjectBaseCharacter>(ActorInfo->AvatarActor))
	{
		if (Character->RightWeapon)
			Character->RightWeapon->TargetActor.Empty();
		if (Character->LeftWeapon)
			Character->LeftWeapon->TargetActor.Empty();
	}

}

UProjectCAbilitySystemComponent* UBaseGameplayAbility::GetProjectCAbilityComponent()
{
	UProjectCAbilitySystemComponent* ASC = Cast<UProjectCAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	return ASC;
}

