// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/ProjectCAttributeSet.h"
#include "ASC/ProjectCGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/ProjectPlayerCharacter.h"
#include "Character/ProjectEnemyCharacter.h"
#include "DebugHelper.h"

UProjectCAttributeSet::UProjectCAttributeSet()
{
	// 새로 설정하려면 세이브 폴더 지우고 값 변경할 것!
	InitExecutionThreshold(0.f);
	InitExecutionGauge(0.f);
	InitBleedThreshold(0.f);
	InitBleedGauge(0.f);
	InitVigor(10.f);
	InitMind(5.f);
	InitEndurance(5.f);
	InitStrength(10.f);
	InitIntelligence(10.f);
	InitMaxHealth(100 + GetVigor() * 3);
	InitCurrentHealth(GetMaxHealth());
	InitMaxStamina(100.0f);
	InitCurrentStamina(100.0f);
	InitMaxMana(100.0f);
	InitCurrentMana(GetMaxMana());
	InitAttackPower(10.0f);
	InitDefencePower(4.f);
	InitMagicPower(10.f);
	InitMagicResistance(3.f);
	InitRune(0.f);
	InitLevel(1);
}

void UProjectCAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetCurrentHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetCurrentStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}

	if (Attribute == GetCurrentManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	// UI 표기용 룬 값 갱신
	if (Attribute == GetRuneAttribute())
	{
		if (Cast<AProjectPlayerCharacter>(GetOwningActor()))
		{
			Cast<AProjectPlayerCharacter>(GetOwningActor())->SetRune(NewValue);

		}
	}

	// 출혈 효과음 재생, 데미지 계산은 GEEC에서 함.
	if (Attribute == GetBleedGaugeAttribute() && Cast<AProjectEnemyCharacter>(GetOwningActor()))
	{
		if (GetBleedGauge() >= GetBleedThreshold())
		{
			GetOwningAbilitySystemComponent()->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_BloodExplosion);
			FGameplayEventData Payload;
			//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), ProjectCGameplayTags::Enemy_Event_ExecutionAvailable, Payload);
		}
	}
}

void UProjectCAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// 죽음 이벤트 태그 전달
	if (GetCurrentHealth() <= 0)
	{
		FGameplayEventData Payload;

		if (!GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_Death))
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), ProjectCGameplayTags::Shared_Event_Death, Payload);

		}

	}

	// 가드 스태미너 부족하면 가드 브레이크 태그 전달
	if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute() && GetCurrentStamina() < 1.f && GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_Block))
	{
		FGameplayEventData Payload;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), ProjectCGameplayTags::Shared_Event_GuardBreak, Payload);

		FGameplayCueParameters Parameters;
		Parameters.Location = GetOwningActor()->GetActorLocation();
		GetOwningAbilitySystemComponent()->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Block, Parameters);
		GetOwningAbilitySystemComponent()->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_BlockBreak, Parameters);


		//Debug::Print("Break", GetCurrentStamina());

	} // 가드 성공 태그 전달
	else if (Data.EvaluatedData.Magnitude < 0.f && Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute() && GetCurrentStamina() > 2 && GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_Block))
	{
		FGameplayEventData Payload;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), ProjectCGameplayTags::Shared_Event_HitReact_Block, Payload);

		FGameplayCueParameters Parameters;
		Parameters.Location = GetOwningActor()->GetActorLocation();
		GetOwningAbilitySystemComponent()->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Block, Parameters);

		//Debug::Print("Guard", GetCurrentStamina());

	}

	// 그로기 처형 및 효과음 재생
	if (Data.EvaluatedData.Attribute == GetExecutionGaugeAttribute() && Cast<AProjectEnemyCharacter>(GetOwningActor()))
	{
		if (GetExecutionGauge() >= GetExecutionThreshold())
		{
			GetOwningAbilitySystemComponent()->AddLooseGameplayTag(ProjectCGameplayTags::Enemy_Status_ExecutionAvailable);
			GetOwningAbilitySystemComponent()->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_EnterExecute);
			FGameplayEventData Payload;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), ProjectCGameplayTags::Enemy_Event_ExecutionAvailable, Payload);
		}
	}

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		if (GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(ProjectCGameplayTags::Enemy_Status_ExecutionAvailable))
		{
			SetExecutionGauge(0.f);
		}

		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
	{
		SetCurrentStamina(FMath::Clamp(GetCurrentStamina(), 0.f, GetMaxStamina()));
	}

	if (Data.EvaluatedData.Attribute == GetCurrentManaAttribute())
	{
		SetCurrentMana(FMath::Clamp(GetCurrentMana(), 0.f, GetMaxMana()));
	}

}

