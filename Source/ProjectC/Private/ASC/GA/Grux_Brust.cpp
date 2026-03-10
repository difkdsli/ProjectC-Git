// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/GA/Grux_Brust.h"
#include "Projectile/EnemyProjectile.h"
#include "Character/ProjectEnemyCharacter.h"
#include "Tasks/GameplayTask_SpawnActor.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGrux_Brust::UGrux_Brust()
{
}

void UGrux_Brust::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UWorld* World = GetWorld();
	if (World && MagmaClass)
	{
		// 범위 공격 Class 스폰
		FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
		SpawnLocation.Z = SpawnLocation.Z + TestZ;
		FRotator SpawnRotation = GetAvatarActorFromActorInfo()->GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetAvatarActorFromActorInfo();
		World->SpawnActor<AEnemyProjectile>(MagmaClass, SpawnLocation, SpawnRotation, SpawnParams);

		// 몽타주 재생
		UAbilityTask_PlayMontageAndWait* PlayMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SkillAnimMontage);
		PlayMontage->OnCompleted.AddDynamic(this, &UGrux_Brust::OnMontageFinished);
		PlayMontage->ReadyForActivation();
	}
}

void UGrux_Brust::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGrux_Brust::OnMontageFinished()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
