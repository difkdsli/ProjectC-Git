// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/EnemyGameplayAbility.h"
#include "Character/ProjectEnemyCharacter.h"


AProjectEnemyCharacter* UEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
    return Cast<AProjectEnemyCharacter>(GetAvatarActorFromActorInfo());
}


