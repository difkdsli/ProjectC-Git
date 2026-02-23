// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/ProjectC_GEEC.h"
#include "ASC/ProjectCAttributeSet.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ASC/ProjectCGameplayTags.h"
#include "DebugHelper.h"

struct FProjectCDamageCapture
{
	// 1) 매크로로 캡처할 Attribute 정의
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefencePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth)
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxStamina)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentStamina)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ExecutionGauge)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BleedGauge)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BleedThreshold)
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance)


	FProjectCDamageCapture()
	{
		// 2) 실제 Capture 정의
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectCAttributeSet, AttackPower, Source, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectCAttributeSet, MagicPower, Source, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectCAttributeSet, DefencePower, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectCAttributeSet, MaxHealth, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectCAttributeSet, CurrentHealth, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectCAttributeSet, MaxStamina, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectCAttributeSet, CurrentStamina, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectCAttributeSet, ExecutionGauge, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectCAttributeSet, BleedGauge, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectCAttributeSet, BleedThreshold, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectCAttributeSet, MagicResistance, Target, false)
	}
};

static const FProjectCDamageCapture& GetProjectCDamageCapture()
{
	static FProjectCDamageCapture CaptureVars;
	return CaptureVars;
}

UProjectC_GEEC::UProjectC_GEEC()
{
	RelevantAttributesToCapture.Add(GetProjectCDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetProjectCDamageCapture().DefencePowerDef);
	RelevantAttributesToCapture.Add(GetProjectCDamageCapture().MagicPowerDef);
	RelevantAttributesToCapture.Add(GetProjectCDamageCapture().MagicResistanceDef);
	RelevantAttributesToCapture.Add(GetProjectCDamageCapture().MaxHealthDef);
	RelevantAttributesToCapture.Add(GetProjectCDamageCapture().CurrentHealthDef);
	RelevantAttributesToCapture.Add(GetProjectCDamageCapture().MaxStaminaDef);
	RelevantAttributesToCapture.Add(GetProjectCDamageCapture().CurrentStaminaDef);
	RelevantAttributesToCapture.Add(GetProjectCDamageCapture().ExecutionGaugeDef);
	RelevantAttributesToCapture.Add(GetProjectCDamageCapture().BleedGaugeDef);
	RelevantAttributesToCapture.Add(GetProjectCDamageCapture().BleedThresholdDef);
}

void UProjectC_GEEC::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Spec 및 태그 정보 가져오기
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
//
	// 캡처된 값 꺼내기
	float AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectCDamageCapture().AttackPowerDef, EvaluateParams, AttackPower);

	float DefencePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectCDamageCapture().DefencePowerDef, EvaluateParams, DefencePower);

	float MagicPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectCDamageCapture().MagicPowerDef, EvaluateParams, MagicPower);

	float MagicResistancePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectCDamageCapture().MagicResistanceDef, EvaluateParams, MagicResistancePower);

	float MaxStamina = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectCDamageCapture().MaxStaminaDef, EvaluateParams, MaxStamina);
	float CurrentStamina = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectCDamageCapture().CurrentStaminaDef, EvaluateParams, CurrentStamina);

	float MaxHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectCDamageCapture().MaxHealthDef, EvaluateParams, MaxHealth);
	float CurrentHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectCDamageCapture().CurrentHealthDef, EvaluateParams, CurrentHealth);

	float BleedGauge = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectCDamageCapture().BleedGaugeDef, EvaluateParams, BleedGauge);
	float BleedThreshold = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectCDamageCapture().BleedThresholdDef, EvaluateParams, BleedThreshold);

	// SetByCaller 데미지 가져오기
	float WeaponDamage = Spec.GetSetByCallerMagnitude(ProjectCGameplayTags::Data_Damage, false);

	// 최종 데미지 계산
	float FinalDamage{ 0 };
	float StaminaDamage{ 0 };
	float ExecutionGauge{ 0 };

	// 방어 성공 시 데미지 감소
	if (ExecutionParams.GetTargetAbilitySystemComponent()->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_BlockSuccess))
	{
		// 물리 데미지 방어 공식
		if (ExecutionParams.GetTargetAbilitySystemComponent()->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_DamageTaken))
		{
			StaminaDamage = WeaponDamage + AttackPower - DefencePower;
			StaminaDamage *= 1.2;
			StaminaDamage = FMath::Max(0.f, StaminaDamage);

			FinalDamage = 0;
			FinalDamage = FMath::Max(0.f, FinalDamage);
			//Debug::Print("MeleeBlockSuccess");

		} // 마법 데미지 방어 공식
		else if (ExecutionParams.GetTargetAbilitySystemComponent()->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_MagicDamageTaken))
		{
			StaminaDamage = WeaponDamage + MagicPower - MagicResistancePower;
			StaminaDamage *= 1.2;
			StaminaDamage = FMath::Max(0.f, StaminaDamage);

			FinalDamage = 0;
			FinalDamage = FMath::Max(0.f, FinalDamage);
			//Debug::Print("MagicBlockSuccess");
		}

	}
	else if (ExecutionParams.GetTargetAbilitySystemComponent()->HasMatchingGameplayTag(ProjectCGameplayTags::Enemy_Status_Executed))
	{
		// 처형 데미지 공식
		if (AttackPower >= MagicPower)
		{
			FinalDamage = AttackPower * 2.0f + MaxHealth * 0.05f;
		}
		else
		{
			FinalDamage = MagicPower * 2.0f + MaxHealth * 0.05f;
		}
		//Debug::Print("ExecutedDamage");
	}
	else if (ExecutionParams.GetTargetAbilitySystemComponent()->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_MagicDamageTaken))
	{
	  // 마법 피해 공식
	  // WeaponDamage: 스킬 자체에서 넘겨준 "기본 마법 피해량"
	  // MagicPower: 시전자 마법 공격력
	  // MagicResistancePower: 피격자 마법 저항

		FinalDamage = WeaponDamage + MagicPower * 1.2f; // 스킬 계수 (1.2배)
		FinalDamage -= MagicResistancePower * 0.8f;     // 방어력 보정 (0.8배)

		// 최소 0 이상 보장
		FinalDamage = FMath::Max(0.f, FinalDamage);

		// 마법도 처형 게이지 영향을 주고 싶다면
		ExecutionGauge += FinalDamage * 0.25f;
		//Debug::Print("MageDamage");
	}
	else
	{
		// 기본 데미지
		FinalDamage = WeaponDamage + AttackPower - DefencePower;
		FinalDamage = FMath::Max(0.f, FinalDamage);

		// 강 공격 시 1.5배 데미지
		if (ExecutionParams.GetSourceAbilitySystemComponent()->HasMatchingGameplayTag(ProjectCGameplayTags::Player_Status_HeavyAttack) || ExecutionParams.GetSourceAbilitySystemComponent()->HasMatchingGameplayTag(ProjectCGameplayTags::Player_Status_AshesOfWar_ShieldRush))
		{
			FinalDamage *= 1.5f;
		}
		//Debug::Print("MeleeDamage");
		ExecutionGauge += FinalDamage * 0.3;

		// 출혈 게이지 누적
		if (ExecutionParams.GetSourceAbilitySystemComponent()->HasMatchingGameplayTag(ProjectCGameplayTags::Player_Equip_Sword))
		{
			// 기본적으로 최종 데미지의 40% 정도를 출혈치로 쌓는다고 가정
			float Bleed = FinalDamage * 0.4f;

			// 출혈 터짐 조건
			if (BleedGauge >= BleedThreshold && BleedThreshold > 0.f)
			{
				// 출혈 데미지 = 대상 최대 체력의 15%
				FinalDamage += MaxHealth * 0.15f;

				OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetProjectCDamageCapture().BleedGaugeProperty, EGameplayModOp::Override, 0.f));

			}
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetProjectCDamageCapture().BleedGaugeProperty, EGameplayModOp::Additive, Bleed));

		}
	}
	//Debug::Print("WeaponDamage : ", WeaponDamage);
	//Debug::Print("FinalDamage : ", FinalDamage);
	//Debug::Print("StaminaDamage : ", StaminaDamage);

	// 체력바 UI에 데미지 표시
	float CurrentDamageStack = Cast<UProjectCAbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent())->DamageStack;
	CurrentDamageStack += FinalDamage;

	if (CurrentHealth <= FinalDamage)
	{
		Cast<UProjectCAbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent())->SetDamageStackTimer(FMath::Clamp(CurrentDamageStack, 0, CurrentHealth));
	}
	else
	{
		Cast<UProjectCAbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent())->SetDamageStackTimer(FMath::Clamp(CurrentDamageStack, 0, MaxHealth));
	}

	ExecutionParams.GetTargetAbilitySystemComponent()->RemoveLooseGameplayTag(ProjectCGameplayTags::Shared_Status_BlockSuccess);

	// 체력 감소 적용 (음수로)
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetProjectCDamageCapture().CurrentHealthProperty, EGameplayModOp::Additive, -FinalDamage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetProjectCDamageCapture().CurrentStaminaProperty, EGameplayModOp::Additive, -StaminaDamage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetProjectCDamageCapture().ExecutionGaugeProperty, EGameplayModOp::Additive, ExecutionGauge));

	

}
