// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_ActivateAbility.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "AIController.h"
#include "Character/ProjectBaseCharacter.h"

UBTTask_ActivateAbility::UBTTask_ActivateAbility()
{
	NodeName = TEXT("Activate Ability By Tag");
}

EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	AActor* ControlledPawn = AICon->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Failed;

	// 캐릭터 → ASC 가져오기
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(ControlledPawn))
	{
		if (UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent())
		{
			if (AbilityTag.IsValid())
			{
				FGameplayTagContainer TagContainer(AbilityTag);

				bool bSuccess = ASC->TryActivateAbilitiesByTag(TagContainer, true);

				return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
			}
		}
	}

	return EBTNodeResult::Failed;

}
