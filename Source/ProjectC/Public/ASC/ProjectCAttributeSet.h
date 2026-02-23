// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ProjectCAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class PROJECTC_API UProjectCAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	

public:


public:
	UProjectCAttributeSet();

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Rune;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, Rune)
		
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, Vigor)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Mind;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, Mind)	
		
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, Endurance)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, Intelligence)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData ExecutionGauge;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, ExecutionGauge)
		
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData ExecutionThreshold;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, ExecutionThreshold)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BleedGauge;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, BleedGauge)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BleedThreshold;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, BleedThreshold)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData CurrentStamina;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, CurrentStamina)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData CurrentMana;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, CurrentMana)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData DefencePower;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, DefencePower)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MagicPower;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, MagicPower)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MagicResistance;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, MagicResistance)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UProjectCAttributeSet, Level)

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)override;;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)override;
};
