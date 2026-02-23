// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ProjectBaseCharacter.h"
#include "Components/InputComponent.h"
#include "Components/ProjectCEnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapon/WeaponBaseActor.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ASC/ProjectCAttributeSet.h"
#include "ASC/BaseGameplayAbility.h"
#include "ASC/PlayerGameplayAbility.h"
#include "ASC/ProjectCGameplayTags.h"
#include "DataAsset/GADataAsset.h"
#include "DataAsset/InputDataAsset.h"
#include "GameplayEffectTypes.h"
#include "DebugHelper.h"
#include "Widget/BaseUserWidget.h"
#include "Weapon/PlayerWeaponBase.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"

AProjectBaseCharacter::AProjectBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = bTick;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	ProjectCAbilitySystemComponent = CreateDefaultSubobject<UProjectCAbilitySystemComponent>(TEXT("ProjectCASC"));
	ProjectCAttributeSet = CreateDefaultSubobject<UProjectCAttributeSet>(TEXT("ProjectCAttributeSet"));

	if (ProjectCAbilitySystemComponent && ProjectCAttributeSet)
	{
		ProjectCAbilitySystemComponent->AddAttributeSetSubobject(ProjectCAttributeSet.Get());
	}
}


void AProjectBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon();

	if (GADataAsset)
		GADataAsset->GrantedAbilities(ProjectCAbilitySystemComponent, 1);

	InitAbilitySystem();

	RegisterAttributeDelegates();
}

void AProjectBaseCharacter::RegisterAttributeDelegates()
{
	ProjectCAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(
		UProjectCAttributeSet::GetCurrentHealthAttribute()).AddUObject(this, &ThisClass::OnAnyAttributeChanged
	);

	ProjectCAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(
		UProjectCAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnAnyAttributeChanged
	);
}

void AProjectBaseCharacter::InitAbilitySystem()
{
	if (ProjectCAbilitySystemComponent)
	{
		ProjectCAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AProjectBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilitySystem();
}

void AProjectBaseCharacter::EquipWeapon()
{
	// 오른 손 무기 설정
	if (GetWorld() && EquipWeaponClass)
	{
		AWeaponBaseActor* PlayerWeapon = Cast<AWeaponBaseActor>(GetWorld()->SpawnActor(EquipWeaponClass));

		PlayerWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocketName);
		CurrentEquipWeapon = PlayerWeapon;
		CurrentEquipWeapon->SetOwner(this);

		// 시작 시 손에 들고 있는 무기는 검
		RightWeapon = CurrentEquipWeapon;
	}
	// 왼손 무기 설정
	if (GetWorld() && LeftEquipWeaponClass)
	{
		AWeaponBaseActor* PlayerWeapon = Cast<AWeaponBaseActor>(GetWorld()->SpawnActor(LeftEquipWeaponClass));

		PlayerWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, LeftWeaponSocketName);
		CurrentLeftEquipWeapon = PlayerWeapon;
		CurrentLeftEquipWeapon->SetOwner(this);


		LeftWeapon = CurrentLeftEquipWeapon;
	}

	if (GetWorld() && EquipShieldClass)
	{
		AWeaponBaseActor* PlayerShield = Cast<AWeaponBaseActor>(GetWorld()->SpawnActor(EquipShieldClass));

		PlayerShield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ShieldSocketName);
		CurrentEquipShield = PlayerShield;
		CurrentEquipShield->SetOwner(this);
	}

	if (GetWorld() && EquipStaffClass)
	{
		AWeaponBaseActor* PlayerStaff = Cast<AWeaponBaseActor>(GetWorld()->SpawnActor(EquipStaffClass));

		PlayerStaff->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, StaffSocketName);
		CurrentEquipStaff = PlayerStaff;
		CurrentEquipStaff->SetOwner(this);
	}
}


void AProjectBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 캐릭터 속도를 가져옵니다.
	const float GroundSpeed = GetVelocity().Size();

	// 멈춰있을 때(속도가 0)는 카메라 방향으로 자동 회전하는 기능을 끄고,
	// 움직일 때는 다시 켭니다.
	GetCharacterMovement()->bUseControllerDesiredRotation = (GroundSpeed > 0.0f);
}

void AProjectBaseCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetCharacterMovement()->IsFalling() && ProjectCAbilitySystemComponent)
	{
		ProjectCAbilitySystemComponent->AddLooseGameplayTag(ProjectCGameplayTags::Shared_Status_InAir);
	}
	else if (!GetCharacterMovement()->IsFalling() && ProjectCAbilitySystemComponent)
	{
		ProjectCAbilitySystemComponent->RemoveLooseGameplayTag(ProjectCGameplayTags::Shared_Status_InAir);
	}
}


void AProjectBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AProjectBaseCharacter::OnAnyAttributeChanged(const FOnAttributeChangeData& Data)
{
}
