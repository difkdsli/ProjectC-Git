// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectCGameInstance.generated.h"

class UProjectCSaveGame;

/**
 * 
 */
UCLASS()
class PROJECTC_API UProjectCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:    
	
	UPROPERTY(BlueprintReadWrite)
	UProjectCSaveGame* ProjectCSaveGame;
};
