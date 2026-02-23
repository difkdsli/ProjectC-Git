// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ProjectPlayerCharacter.h"
#include "DataAsset/InputDataAsset.h"
#include "Components/ProjectCEnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ASC/ProjectCAttributeSet.h"
#include "ASC/ProjectCGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Widget/PlayerUserWidget.h"
#include "Weapon/PlayerWeaponBase.h"
#include "DataAsset/ItemDataAsset.h"
#include "Projectile/PlayerProjectile.h"
#include "DebugHelper.h"
#include "NiagaraComponent.h"

AProjectPlayerCharacter::AProjectPlayerCharacter()
{
	DodgeAfterEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DodgeAfterEffect"));
	DodgeAfterEffect->SetupAttachment(RootComponent);
}

void AProjectPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (ScreenBloodWidgetClass)
	{
		ScreenBloodWidget = CreateWidget<UPlayerUserWidget>(GetWorld(), ScreenBloodWidgetClass);
		ScreenBloodWidget->AddToViewport();
		ScreenBloodWidget->UIOwnerASC = ProjectCAbilitySystemComponent;

		if (!ScreenBloodWidget)return;
	}
	if (PlayerUIClass)
	{
		PlayerUIWidget = CreateWidget<UPlayerUserWidget>(GetWorld(), PlayerUIClass);
		PlayerUIWidget->AddToViewport();
		PlayerUIWidget->UIOwnerASC = ProjectCAbilitySystemComponent;

		if (!PlayerUIWidget)return;

		PlayerUIWidget->MaxHealth = ProjectCAttributeSet->GetMaxHealth();
		PlayerUIWidget->CurrentHealth = ProjectCAttributeSet->GetCurrentHealth();
		PlayerUIWidget->MaxMana = ProjectCAttributeSet->GetMaxMana();
		PlayerUIWidget->CurrentMana = ProjectCAttributeSet->GetCurrentMana();
		PlayerUIWidget->MaxStamina = ProjectCAttributeSet->GetMaxStamina();
		PlayerUIWidget->CurrentStamina = ProjectCAttributeSet->GetCurrentStamina();
	}

	if (CurrentEquipWeapon == RightWeapon)
	{
		Cast<APlayerWeaponBase>(CurrentEquipWeapon)->bWeaponHidden = false;
		ProjectCAbilitySystemComponent->AddLooseGameplayTag(ProjectCGameplayTags::Player_Equip_Sword);
	}

	if (ItemDataAsset)
	{
		for (auto& Item : ItemDataAsset->Potion)
		{
			if (!CurrentPotion.IsValid())
			{
				CurrentPotion = Item;
				continue;
			}

			if (!NextPotion.IsValid())
			{
				NextPotion = Item;
			}
		}
	}

	if (MagicDataAsset)
	{
		for (auto& Magic : MagicDataAsset->Magic)
		{
			if (!CurrentMagic.IsValid())
			{
				CurrentMagic = Magic;
				continue;
			}

			if (!NextMagic.IsValid())
			{
				NextMagic = Magic;
			}
		}
	}

	if (CurrentMagic.IsValid())
	{
		ProjectCAbilitySystemComponent->AddLooseGameplayTag(CurrentMagic.MagicTag);
	}
}

void AProjectPlayerCharacter::PlayBloodAnim()
{
	// 체력이 20% 이하일 때 피격 당하면 화면 테두리에 효과 발생
	if (ScreenBloodWidget && bIsLowHealth &&ProjectCAbilitySystemComponent->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_DamageTaken)||ProjectCAbilitySystemComponent->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_MagicDamageTaken))
	{
		ScreenBloodWidget->PlayBloodAnim();
	}
}

void AProjectPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsFreeForAction())
	{
		float CurrentStamina = ProjectCAttributeSet->GetCurrentStamina();
		ProjectCAttributeSet->SetCurrentStamina(CurrentStamina + 0.2f);
	}
	else if (ProjectCAbilitySystemComponent->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_Block))
	{
		float CurrentStamina = ProjectCAttributeSet->GetCurrentStamina();
		ProjectCAttributeSet->SetCurrentStamina(CurrentStamina + 0.05f);
	}
	else if (ProjectCAbilitySystemComponent->HasMatchingGameplayTag(ProjectCGameplayTags::Player_Status_Sprint))
	{
		float CurrentStamina = ProjectCAttributeSet->GetCurrentStamina();
		ProjectCAttributeSet->SetCurrentStamina(CurrentStamina - 0.15f);
	}
}

bool AProjectPlayerCharacter::IsFreeForAction()
{
	return !ProjectCAbilitySystemComponent->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_InAction) &&
		!ProjectCAbilitySystemComponent->HasMatchingGameplayTag(ProjectCGameplayTags::Player_Status_Sprint);
}

void AProjectPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputDataAsset->CharacterMappingContext, 0);
		}
	}

	if (UProjectCEnhancedInputComponent* EnhancedInputComponent = Cast<UProjectCEnhancedInputComponent>(
		PlayerInputComponent))
	{
		EnhancedInputComponent->NativeBind(InputDataAsset, ProjectCGameplayTags::InputTag_Move,
		                                   ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->NativeBind(InputDataAsset, ProjectCGameplayTags::InputTag_Look,
		                                   ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->NativeBind(InputDataAsset, ProjectCGameplayTags::InputTag_SwitchTarget,
		                                   ETriggerEvent::Started, this, &ThisClass::SwitchTarget);

		EnhancedInputComponent->AbilityBind(InputDataAsset, this, &ThisClass::AbilityInputPressed,
		                                    &ThisClass::AbilityInputReleased);
	}
}

void AProjectPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator ControllerRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, ControllerRotation.Yaw, 0);

		const FVector ForwardDireciton = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDireciton = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDireciton, MovementVector.X); // W S ������
		AddMovementInput(RightDireciton, MovementVector.Y); // A D ������
	}
}

void AProjectPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// X�� (�¿�) �Է� -> Yaw (�¿�) ȸ��
		AddControllerYawInput(LookAxisVector.X);

		// Y�� (����) �Է� -> Pitch (����) ȸ��
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProjectPlayerCharacter::SwitchTarget()
{
	FGameplayEventData Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, ProjectCGameplayTags::Player_Event_TargetSwitch,
	                                                         Payload);
}

void AProjectPlayerCharacter::AbilityInputPressed(FGameplayTag InInputTag)
{
	ProjectCAbilitySystemComponent->OnAbilityInputPressed(InInputTag);

	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("AbilityInputPressed"));
}

void AProjectPlayerCharacter::AbilityInputReleased(FGameplayTag InInputTag)
{
	ProjectCAbilitySystemComponent->OnAbilityInputReleased(InInputTag);

	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("AbilityInputPressed"));
}

void AProjectPlayerCharacter::ActiveDodgeEffect()
{
	if (DodgeAfterEffect)
	{
		DodgeAfterEffect->Activate();
	}
}

void AProjectPlayerCharacter::DeactiveDodgeEffect()
{
	if (DodgeAfterEffect)
	{
		DodgeAfterEffect->Deactivate();
	}
}

void AProjectPlayerCharacter::OnAnyAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (Data.Attribute == ProjectCAttributeSet->GetMaxHealthAttribute())
	{
		PlayerUIWidget->MaxHealth = Data.NewValue;
	}

	if (Data.Attribute == ProjectCAttributeSet->GetCurrentHealthAttribute())
	{
		PlayerUIWidget->CurrentHealth = Data.NewValue;
	}

	if (Data.Attribute == ProjectCAttributeSet->GetMaxStaminaAttribute())
	{
		PlayerUIWidget->MaxStamina = Data.NewValue;
	}

	if (Data.Attribute == ProjectCAttributeSet->GetCurrentStaminaAttribute())
	{
		PlayerUIWidget->CurrentStamina = Data.NewValue;
	}

	if (Data.Attribute == ProjectCAttributeSet->GetMaxManaAttribute())
	{
		PlayerUIWidget->MaxMana = Data.NewValue;
	}

	if (Data.Attribute == ProjectCAttributeSet->GetCurrentManaAttribute())
	{
		PlayerUIWidget->CurrentMana = Data.NewValue;
	}
}


void AProjectPlayerCharacter::SetRune(int NewValue)
{
	PrevRune = CurrentRune;
	CurrentRune = NewValue;
	LootedRunes = CurrentRune - PrevRune;

	OnUpdateRuneDelegate.Broadcast(PrevRune, CurrentRune, LootedRunes);
}

void AProjectPlayerCharacter::RegisterAttributeDelegates()
{
	Super::RegisterAttributeDelegates();

	ProjectCAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(
		UProjectCAttributeSet::GetCurrentStaminaAttribute()).AddUObject(this, &ThisClass::OnAnyAttributeChanged
	);

	ProjectCAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(
		UProjectCAttributeSet::GetMaxStaminaAttribute()).AddUObject(this, &ThisClass::OnAnyAttributeChanged
	);


	ProjectCAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(
		UProjectCAttributeSet::GetCurrentManaAttribute()).AddUObject(this, &ThisClass::OnAnyAttributeChanged
	);

	ProjectCAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(
		UProjectCAttributeSet::GetMaxManaAttribute()).AddUObject(this, &ThisClass::OnAnyAttributeChanged
	);
}
