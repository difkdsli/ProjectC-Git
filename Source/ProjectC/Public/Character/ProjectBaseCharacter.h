// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemInterface.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ProjectBaseCharacter.generated.h"

class UInputDataAsset;
class USpringArmComponent;
class UCameraComponent;
class AWeaponBaseActor;
class UProjectCAbilitySystemComponent;
class UProjectCAttributeSet;
class UGADataAsset;
class UProjectCEnhancedInputComponent;
class UBaseUserWidget;
class UAbilityAsync_WaitGameplayEvent;

UCLASS()
class PROJECTC_API AProjectBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AProjectBaseCharacter();

	UPROPERTY(EditAnywhere)
	bool bTick = false;

protected:
	virtual void BeginPlay() override;

	void InitAbilitySystem();
	virtual void PossessedBy(AController* NewController) override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

public: // InputSetting

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputDataAsset> InputDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	UGADataAsset* GADataAsset;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

public: // Weapon

	void EquipWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeaponBaseActor> EquipWeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeaponBaseActor> LeftEquipWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeaponBaseActor> EquipShieldClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeaponBaseActor> EquipStaffClass;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWeaponBaseActor* CurrentEquipWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWeaponBaseActor* CurrentLeftEquipWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWeaponBaseActor* CurrentEquipShield;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWeaponBaseActor* CurrentEquipStaff;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	AWeaponBaseActor* RightWeapon;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	AWeaponBaseActor* LeftWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName LeftWeaponSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName ShieldSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName StaffSocketName;

public: // Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UProjectCAbilitySystemComponent> ProjectCAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UProjectCAttributeSet> ProjectCAttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UProjectCEnhancedInputComponent* ProjectCEnhancedInputComponent;

public: // Interface

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return ProjectCAbilitySystemComponent;
	}

public: // AttributeSet

	virtual void RegisterAttributeDelegates();
	virtual void OnAnyAttributeChanged(const FOnAttributeChangeData& Data);
};
