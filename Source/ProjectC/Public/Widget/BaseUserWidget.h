// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

class UProjectCAbilitySystemComponent;
class AProjectBaseCharacter;
/**
 * 
 */
UCLASS()
class PROJECTC_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct()override;

public: // AttributeSet

	UPROPERTY(BlueprintReadOnly)
	float MaxHealth{7};

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth{7};

public:

	UPROPERTY(BlueprintReadWrite)
	UProjectCAbilitySystemComponent* UIOwnerASC;

	UPROPERTY(BlueprintReadOnly)
	AProjectBaseCharacter* OwningBaseCharacter;
};
