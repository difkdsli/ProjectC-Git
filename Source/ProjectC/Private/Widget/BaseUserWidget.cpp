// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BaseUserWidget.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "Character/ProjectBaseCharacter.h"

void UBaseUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OwningBaseCharacter = Cast<AProjectBaseCharacter>(GetOwningPlayerPawn());
	UIOwnerASC = OwningBaseCharacter->ProjectCAbilitySystemComponent;
}

void UBaseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

}
