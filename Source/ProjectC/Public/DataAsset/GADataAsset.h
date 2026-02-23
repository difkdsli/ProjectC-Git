// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GADataAsset.generated.h"


class UBaseGameplayAbility;
class UProjectCAbilitySystemComponent;


USTRUCT(BlueprintType)
struct FAbilityInputMapping
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBaseGameplayAbility> AbilityToGrant;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

};
/**
 *
 */
UCLASS()
class PROJECTC_API UGADataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()


public:


	UPROPERTY(EditAnywhere, Meta = (DisplayName = "On Given Abilities"))
	TArray<TSubclassOf<UBaseGameplayAbility>> OnGivenGA;

	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Release Abilities"))
	TArray<FAbilityInputMapping > ReleaseGA;

	void GrantedAbilities(UProjectCAbilitySystemComponent* InASC, int32 ApplyLevel);

};
