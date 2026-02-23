// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/ProjectCSaveGame.h"
#include "GameplayEffect.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ASC/ProjectCAttributeSet.h"
#include "DataAsset/ItemDataAsset.h"
#include "Character/ProjectPlayerCharacter.h"

void UProjectCSaveGame::SaveItemDataAsset(UItemDataAsset* SaveItemDataAsset)
{
	HealthPotionNum = SaveItemDataAsset->Potion[0].InGameItemNum;
	ManaPotionNum = SaveItemDataAsset->Potion[1].InGameItemNum;

}

void UProjectCSaveGame::LoadItemDataAsset(UItemDataAsset* LoadItemDataAsset, AProjectPlayerCharacter* Player)
{
	LoadItemDataAsset->Potion[0].InGameItemNum = HealthPotionNum;
	LoadItemDataAsset->Potion[1].InGameItemNum = ManaPotionNum;

	if (Player->CurrentPotion.ItemTag == LoadItemDataAsset->Potion[0].ItemTag)
	{
		Player->CurrentPotion.InGameItemNum = HealthPotionNum;
	}
	else
	{
		Player->CurrentPotion.InGameItemNum = ManaPotionNum;
	}

	if (Player->NextPotion.ItemTag == LoadItemDataAsset->Potion[1].ItemTag)
	{
		Player->NextPotion.InGameItemNum = ManaPotionNum;
	}
	else
	{
		Player->CurrentPotion.InGameItemNum = HealthPotionNum;
	}
}

void UProjectCSaveGame::SaveAttributeSet(UProjectCAttributeSet* ProjectCAttributeSet)
{
	if (!ProjectCAttributeSet) return;

	PlayerStatStruct.Rune = ProjectCAttributeSet->GetRune();
	PlayerStatStruct.Vigor = ProjectCAttributeSet->GetVigor();
	PlayerStatStruct.Mind = ProjectCAttributeSet->GetMind();
	PlayerStatStruct.Endurance = ProjectCAttributeSet->GetEndurance();
	PlayerStatStruct.Strength = ProjectCAttributeSet->GetStrength();
	PlayerStatStruct.Intelligence = ProjectCAttributeSet->GetIntelligence();
	PlayerStatStruct.ExecutionGauge = ProjectCAttributeSet->GetExecutionGauge();
	PlayerStatStruct.ExecutionThreshold = ProjectCAttributeSet->GetExecutionThreshold();
	PlayerStatStruct.BleedGauge = ProjectCAttributeSet->GetBleedGauge();
	PlayerStatStruct.BleedThreshold = ProjectCAttributeSet->GetBleedThreshold();
	PlayerStatStruct.MaxHealth = ProjectCAttributeSet->GetMaxHealth();
	PlayerStatStruct.CurrentHealth = ProjectCAttributeSet->GetCurrentHealth();
	PlayerStatStruct.MaxStamina = ProjectCAttributeSet->GetMaxStamina();
	PlayerStatStruct.CurrentStamina = ProjectCAttributeSet->GetCurrentStamina();
	PlayerStatStruct.MaxMana = ProjectCAttributeSet->GetMaxMana();
	PlayerStatStruct.CurrentMana = ProjectCAttributeSet->GetCurrentMana();
	PlayerStatStruct.AttackPower = ProjectCAttributeSet->GetAttackPower();
	PlayerStatStruct.DefencePower = ProjectCAttributeSet->GetDefencePower();
	PlayerStatStruct.MagicPower = ProjectCAttributeSet->GetMagicPower();
	PlayerStatStruct.MagicResistance = ProjectCAttributeSet->GetMagicResistance();
	PlayerStatStruct.Level = ProjectCAttributeSet->GetLevel();
}

void UProjectCSaveGame::LoadAttributeSet(UProjectCAttributeSet* ProjectCAttributeSet)
{
	if (!ProjectCAttributeSet) return;

	UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(ProjectCAttributeSet->GetOwningAbilitySystemComponent());
	if (!ASC) return;

	// 모든 Attribute를 GAS 방식으로 적용
	TArray<TPair<FGameplayAttribute, float>> AttributesToApply =
	{
		{ UProjectCAttributeSet::GetRuneAttribute(),               PlayerStatStruct.Rune },
		{ UProjectCAttributeSet::GetVigorAttribute(),              PlayerStatStruct.Vigor },
		{ UProjectCAttributeSet::GetMindAttribute(),               PlayerStatStruct.Mind },
		{ UProjectCAttributeSet::GetEnduranceAttribute(),          PlayerStatStruct.Endurance },
		{ UProjectCAttributeSet::GetStrengthAttribute(),           PlayerStatStruct.Strength },
		{ UProjectCAttributeSet::GetIntelligenceAttribute(),       PlayerStatStruct.Intelligence },
		{ UProjectCAttributeSet::GetExecutionGaugeAttribute(),     PlayerStatStruct.ExecutionGauge },
		{ UProjectCAttributeSet::GetExecutionThresholdAttribute(), PlayerStatStruct.ExecutionThreshold },
		{ UProjectCAttributeSet::GetBleedGaugeAttribute(),         PlayerStatStruct.BleedGauge },
		{ UProjectCAttributeSet::GetBleedThresholdAttribute(),     PlayerStatStruct.BleedThreshold },
		{ UProjectCAttributeSet::GetMaxHealthAttribute(),          PlayerStatStruct.MaxHealth },
		{ UProjectCAttributeSet::GetCurrentHealthAttribute(),      PlayerStatStruct.CurrentHealth },
		{ UProjectCAttributeSet::GetMaxStaminaAttribute(),         PlayerStatStruct.MaxStamina },
		{ UProjectCAttributeSet::GetCurrentStaminaAttribute(),     PlayerStatStruct.CurrentStamina },
		{ UProjectCAttributeSet::GetMaxManaAttribute(),            PlayerStatStruct.MaxMana },
		{ UProjectCAttributeSet::GetCurrentManaAttribute(),        PlayerStatStruct.CurrentMana },
		{ UProjectCAttributeSet::GetAttackPowerAttribute(),        PlayerStatStruct.AttackPower },
		{ UProjectCAttributeSet::GetDefencePowerAttribute(),       PlayerStatStruct.DefencePower },
		{ UProjectCAttributeSet::GetMagicPowerAttribute(),         PlayerStatStruct.MagicPower },
		{ UProjectCAttributeSet::GetMagicResistanceAttribute(),    PlayerStatStruct.MagicResistance },
		{ UProjectCAttributeSet::GetLevelAttribute(),              PlayerStatStruct.Level }
	};

	for (const auto& Pair : AttributesToApply)
	{
		// 현재 값과의 차이를 계산
		float CurrentValue = ASC->GetNumericAttribute(Pair.Key);
		float Delta = Pair.Value - CurrentValue;

		if (FMath::Abs(Delta) > KINDA_SMALL_NUMBER)
		{
			ASC->ApplyModToAttribute(Pair.Key, EGameplayModOp::Additive, Delta);
		}
	}
}
