// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ASC/ProjectCGameplayTags.h"
#include "ASC/ProjectCAttributeSet.h"
#include "DebugHelper.h"

void UProjectCAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		if (InInputTag.MatchesTag(ProjectCGameplayTags::InputTag_Toggle) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("CancelAbilityHandle"));
		}
		else
		{
			TryActivateAbility(AbilitySpec.Handle);
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("TryActivateAbility"));
		}
	}
}

void UProjectCAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.MatchesTag(ProjectCGameplayTags::InputTag_Hold))
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	}
}

bool UProjectCAbilitySystemComponent::bHealthCheck()
{
	bool found;
	float CurrentHealth = GetGameplayAttributeValue(UProjectCAttributeSet::GetCurrentHealthAttribute(), found);

	if (CurrentHealth <= 0.f)
	{
		return true;
	}
	else
	{
		return false;

	}
}

void UProjectCAbilitySystemComponent::SetDamageStack()
{
	PreviousDamageStack = DamageStack;
}

void UProjectCAbilitySystemComponent::SetDamageStackTimer(float InDamageStack)
{
	DamageStack = InDamageStack;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::SetDamageStack, 1.0f, false);
}
