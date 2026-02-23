// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseUserWidget.h"
#include "EnemyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTC_API UEnemyUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct()override;

	virtual void NativeOnInitialized() override;
	
};
