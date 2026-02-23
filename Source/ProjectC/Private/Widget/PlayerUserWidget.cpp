// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerUserWidget.h"
#include "Character/ProjectPlayerCharacter.h"
#include "ASC/ProjectCAttributeSet.h"

void UPlayerUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OwningPlayerCharacter = Cast<AProjectPlayerCharacter>(GetOwningPlayerPawn());
	OwningPlayerController = Cast<APlayerController>(OwningPlayerCharacter->GetController());
	OwningAttributeSet = OwningPlayerCharacter->ProjectCAttributeSet;

}

void UPlayerUserWidget::IncreaseAttackPowerValue(float RuneCost)
{
	OwningAttributeSet->SetAttackPower(OwningAttributeSet->GetAttackPower() + 1);
	OwningAttributeSet->SetStrength(OwningAttributeSet->GetStrength() + 1);
	ApplyLevelUpCost(RuneCost);
	IncreaseLevelValue();
}


void UPlayerUserWidget::IncreaseMagicPowerValue(float RuneCost)
{
	OwningAttributeSet->SetMagicPower(OwningAttributeSet->GetMagicPower() + 1);
	OwningAttributeSet->SetIntelligence(OwningAttributeSet->GetIntelligence() + 1);
	ApplyLevelUpCost(RuneCost);
	IncreaseLevelValue();
}

void UPlayerUserWidget::IncreaseHealthValue(float RuneCost)
{
	OwningAttributeSet->SetMaxHealth(OwningAttributeSet->GetMaxHealth() + 15);
	OwningAttributeSet->SetVigor(OwningAttributeSet->GetVigor() + 1);
	ApplyLevelUpCost(RuneCost);
	IncreaseLevelValue();
}

void UPlayerUserWidget::IncreaseManaValue(float RuneCost)
{
	OwningAttributeSet->SetMaxMana(OwningAttributeSet->GetMaxMana() + 10);
	OwningAttributeSet->SetMind(OwningAttributeSet->GetMind() + 1);
	ApplyLevelUpCost(RuneCost);
	IncreaseLevelValue();
}

void UPlayerUserWidget::IncreaseStaminaValue(float RuneCost)
{
	OwningAttributeSet->SetMaxStamina(OwningAttributeSet->GetMaxStamina() + 10);
	OwningAttributeSet->SetEndurance(OwningAttributeSet->GetEndurance() + 1);
	ApplyLevelUpCost(RuneCost);
	IncreaseLevelValue();
}

void UPlayerUserWidget::IncreaseLevelValue()
{
	OwningAttributeSet->SetLevel(OwningAttributeSet->GetLevel() + 1);
}

void UPlayerUserWidget::ApplyLevelUpCost(float RuneCost)
{
	OwningAttributeSet->SetRune(OwningAttributeSet->GetRune() - RuneCost);
}

void UPlayerUserWidget::HealthFlaskUp(int& HealthPotion, int& ManaPotion)
{
	if (OwningPlayerCharacter->ItemDataAsset->Potion[1].DefaultNum > 0)
	{
		OwningPlayerCharacter->ItemDataAsset->Potion[0].DefaultNum++;
		OwningPlayerCharacter->ItemDataAsset->Potion[1].DefaultNum--;
	}
	HealthPotion = OwningPlayerCharacter->ItemDataAsset->Potion[0].DefaultNum;
	ManaPotion = OwningPlayerCharacter->ItemDataAsset->Potion[1].DefaultNum;
}

void UPlayerUserWidget::ManaFlaskUp(int& HealthPotion, int& ManaPotion)
{
	if (OwningPlayerCharacter->ItemDataAsset->Potion[0].DefaultNum > 0)
	{
		OwningPlayerCharacter->ItemDataAsset->Potion[1].DefaultNum++;
		OwningPlayerCharacter->ItemDataAsset->Potion[0].DefaultNum--;
	}
	HealthPotion = OwningPlayerCharacter->ItemDataAsset->Potion[0].DefaultNum;
	ManaPotion = OwningPlayerCharacter->ItemDataAsset->Potion[1].DefaultNum;
}

void UPlayerUserWidget::CurrentFlaskUp(int& HealthPotion, int& ManaPotion)
{
	if (OwningPlayerCharacter->ItemDataAsset)
	{
		HealthPotion = OwningPlayerCharacter->ItemDataAsset->Potion[0].DefaultNum;
		ManaPotion = OwningPlayerCharacter->ItemDataAsset->Potion[1].DefaultNum;
	}
}

void UPlayerUserWidget::DisableInput()
{
	OwningPlayerController->DisableInput(OwningPlayerController);
}

void UPlayerUserWidget::EnableInput()
{
	OwningPlayerController->EnableInput(OwningPlayerController);
}
