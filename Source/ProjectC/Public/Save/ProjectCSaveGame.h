// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProjectCSaveGame.generated.h"

class UProjectCAttributeSet;
class UItemDataAsset;
class AProjectPlayerCharacter;

USTRUCT(BlueprintType)
struct FCharacterStats
{
    GENERATED_BODY()

    // 능력치
    UPROPERTY(BlueprintReadWrite)
    float Rune = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float Vigor = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float Mind = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float Endurance = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float Strength = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float Intelligence = 0.f;

    // 전투 관련
    UPROPERTY(BlueprintReadWrite)
    float ExecutionGauge = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float ExecutionThreshold = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float BleedGauge = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float BleedThreshold = 0.f;

    // HP / 스태미나 / 마나
    UPROPERTY(BlueprintReadWrite)
    float MaxHealth = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float CurrentHealth = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float MaxStamina = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float CurrentStamina = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float MaxMana = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float CurrentMana = 0.f;

    // 공격 / 방어 / 마법
    UPROPERTY(BlueprintReadWrite)
    float AttackPower = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float DefencePower = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float MagicPower = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float MagicResistance = 0.f;

    // 레벨
    UPROPERTY(BlueprintReadWrite)
    float Level = 1.f;
};
/**
 * 
 */
UCLASS()
class PROJECTC_API UProjectCSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

    UPROPERTY(BlueprintReadWrite)
    FCharacterStats PlayerStatStruct;

    UPROPERTY(BlueprintReadWrite)
    FTransform PlayerTransform;

    UPROPERTY(BlueprintReadOnly)
    int HealthPotionNum;

    UPROPERTY(BlueprintReadOnly)
    int ManaPotionNum;

public:

    UFUNCTION(BlueprintCallable)
    void SaveItemDataAsset(UItemDataAsset* SaveItemDataAsset);

    UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Player", HideSelfPin))
    void LoadItemDataAsset(UItemDataAsset* LoadItemDataAsset, AProjectPlayerCharacter* Player);

    UFUNCTION(BlueprintCallable)
    void SaveAttributeSet(UProjectCAttributeSet* ProjectCAttributeSet);

    UFUNCTION(BlueprintCallable)
    void LoadAttributeSet(UProjectCAttributeSet* ProjectCAttributeSet);


};
