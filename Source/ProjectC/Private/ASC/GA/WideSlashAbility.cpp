// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/GA/WideSlashAbility.h"
#include "ASC/ProjectCAttributeSet.h"
#include "ASC/ProjectCGameplayTags.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "Character/ProjectPlayerCharacter.h"
#include "Character/ProjectEnemyCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DebugHelper.h"

void UWideSlashAbility::WideSlashOverlap(AActor* TargetActor)
{
	// 데미지 전달
	if (AProjectEnemyCharacter* SourceActor = GetEnemyCharacterFromActorInfo())
	{

		if (AProjectPlayerCharacter* HitActor = Cast<AProjectPlayerCharacter>(TargetActor))
		{
			// 중복 체크
			if (TargetActorArray.Contains(HitActor))return;

			TargetActorArray.AddUnique(HitActor);
			// 타겟 죽었는 지 체크
			bool AttrubteFindResult;
			float CurrentHealth = HitActor->ProjectCAbilitySystemComponent->GetGameplayAttributeValue(UProjectCAttributeSet::GetCurrentHealthAttribute(), AttrubteFindResult);

			if (AttrubteFindResult && CurrentHealth <= 0)
			{
				return;
			}

			UProjectCAbilitySystemComponent* HitActorASC = HitActor->ProjectCAbilitySystemComponent;
			// 물리 데미지 받음 태그 전달
			HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Shared_Status_DamageTaken);
			UProjectCAbilitySystemComponent* SourceActorASC = SourceActor->ProjectCAbilitySystemComponent;

			// 데미지 전달하기 위한 컨텍스트, 스펙 생성
			FGameplayEffectContextHandle EffectContext = SourceActorASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = SourceActorASC->MakeOutgoingSpec(DamageEffectClass, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				SpecHandle.Data->SetSetByCallerMagnitude(ProjectCGameplayTags::Data_Damage, WideSlashDamage);
			}

			if (HitActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_Invincible))return;


			// 피격 방향 계산
			FVector HitActorToSourceActor = (SourceActor->GetActorLocation() - HitActor->GetActorLocation()).GetSafeNormal(); // 피해자에서 공격자로 가는 방향 벡터
			FVector HitActorForward = HitActor->GetActorForwardVector(); // 피해자의 전방 벡터

			float DotResult = FVector::DotProduct(HitActorForward, HitActorToSourceActor); // 피해자의 전방벡터가 공격자로 향하는 전방벡터와 얼마나 일치하는지. 1은 정면 -1은 후면 0은 좌우 둘 중 하나

			float Direction = FVector::CrossProduct(HitActorForward, HitActorToSourceActor).Z; // 피해자의 전방벡터와 공격자로 향하는 방향 벡터를 외적하여 나온 값으로 오른쪽 왼쪽 판단

			FGameplayTag HitReactTag;
			FGameplayEventData Payload;
			Payload.Instigator = SourceActor;
			Payload.Target = HitActor;

			if (DotResult > 0.5f) // 피해자 정면
			{

				// 방어 중일 경우
				// 스태미너 1 이상이면 방어 성공
				if (HitActor->ProjectCAttributeSet->GetCurrentStamina() > 0.f && HitActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_Block))
				{

					HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Shared_Status_BlockSuccess);
					//HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Block;
					// 방어 성공시 가드 카운터 가능
					HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Player_Status_GuardCounterAvailable);
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				}// 패링 하는 경우
				else if (HitActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Player_Status_Parrying))
				{
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, ProjectCGameplayTags::Player_Event_ParrySuccess, Payload);

					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Front;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(SourceActor, HitReactTag, Payload);
				}
				else
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Front;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

					// 블러드 효과 게임플레이 큐 실행
					FGameplayCueParameters Parameters;
					Parameters.Location = HitActor->GetActorLocation();
					HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);
				}


			}
			else // 피해자 후면
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Back;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				// 블러드 효과 게임플레이 큐 실행
				FGameplayCueParameters Parameters;
				Parameters.Location = HitActor->GetActorLocation();
				HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);
			}
		}
	}

}
