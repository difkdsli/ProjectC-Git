// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/GADataAsset.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ASC/ProjectCGameplayTags.h"
#include "ASC/BaseGameplayAbility.h"
#include "DebugHelper.h"

void UGADataAsset::GrantedAbilities(UProjectCAbilitySystemComponent* InASC, int32 ApplyLevel)
{
	if (InASC)
	{
		if (!InASC)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("return"));
			return;
		}
		for (TSubclassOf<UBaseGameplayAbility> GA : OnGivenGA)
		{
			FGameplayAbilitySpec Spec(GA);
			Spec.SourceObject = InASC->GetAvatarActor();
			Spec.Level = ApplyLevel;
			InASC->GiveAbility(Spec);
			InASC->TryActivateAbility(Spec.Handle);
			InASC->CancelAbilityHandle(Spec.Handle);
		
		}

		for (const FAbilityInputMapping& GA : ReleaseGA)
		{
			FGameplayAbilitySpec Spec(GA.AbilityToGrant);
			Spec.SourceObject = InASC->GetAvatarActor();
			Spec.Level = ApplyLevel;

			if (GA.InputTag.IsValid())
			{
				Spec.DynamicAbilityTags.AddTag(GA.InputTag);

			}
			InASC->GiveAbility(Spec);
		}
	}

}
