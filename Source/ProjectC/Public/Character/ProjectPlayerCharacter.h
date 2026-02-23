// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ProjectBaseCharacter.h"
#include "ASC/ProjectCGameplayTags.h"
#include "DataAsset/ItemDataAsset.h"
#include "DataAsset/MagicDataAsset.h"
#include "ProjectPlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateRune, int, PrevRune, int, CurrentRune, int, LootedRune);

class UPlayerUserWidget;
class AProjectEnemyCharacter;
class UItemDataAsset;
class UMagicDataAsset;
class APlayerProjectile;

/**
 *
 */
UCLASS()
class PROJECTC_API AProjectPlayerCharacter : public AProjectBaseCharacter
{
	GENERATED_BODY()

public:
	AProjectPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	bool IsFreeForAction();
	
public:
	void PlayBloodAnim();
	bool bIsLowHealth = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	float LowHealthPercent = 0.3f; // 피격 효과 언제 터질 지 체력 퍼센트 설정
public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SwitchTarget();

	void AbilityInputPressed(FGameplayTag InInputTag);
	void AbilityInputReleased(FGameplayTag InInputTag);


	void ActiveDodgeEffect();
	void DeactiveDodgeEffect();

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* DodgeAfterEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	class UPlayerUserWidget* ScreenBloodWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UPlayerUserWidget> ScreenBloodWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	UPlayerUserWidget* PlayerUIWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UPlayerUserWidget> PlayerUIClass;

	UPROPERTY(BlueprintReadWrite)
	AProjectEnemyCharacter* CurrentTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UItemDataAsset> ItemDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemSet CurrentPotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int CurrentPotionIndex{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemSet NextPotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int NextPotionIndex{1};

	// UI ǥ���
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FOnUpdateRune OnUpdateRuneDelegate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int PrevRune{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int CurrentRune{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int LootedRunes{0};

	void SetRune(int NewValue);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	TObjectPtr<UMagicDataAsset> MagicDataAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic")
	FMagicSet CurrentMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic")
	FMagicSet NextMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic")
	int CurrentMagicIndex{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic")
	int NextMagicIndex{1};

public: // AttributeSet

	virtual void RegisterAttributeDelegates() override;
	virtual void OnAnyAttributeChanged(const FOnAttributeChangeData& Data) override;
};
