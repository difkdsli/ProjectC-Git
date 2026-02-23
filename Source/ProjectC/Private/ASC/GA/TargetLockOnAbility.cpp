// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/GA/TargetLockOnAbility.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ASC/ProjectCAttributeSet.h"
#include "Character/ProjectEnemyCharacter.h"
#include "Character/ProjectPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "DebugHelper.h"

void UTargetLockOnAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(ProjectCGameplayTags::Player_Status_TargetLock);
	FindLockOnTargetFromTrace();
}

void UTargetLockOnAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (CurrentLockOnTarget)
		CurrentLockOnTarget->bHealthBarTimeOut = true;

	DrawTargetLockOnWidget(false);
	AvailableTargets.Empty();
	CurrentLockOnTarget = nullptr;
	PlayerOrientRotationToMovement(true);

	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(ProjectCGameplayTags::Player_Status_TargetLock);
}

void UTargetLockOnAbility::SetCurrentLockOnTarget(FVector PlayerLocation)
{
	bool bFound;
	if (CurrentLockOnTarget)
	{
		float CurrentTargetHealth = CurrentLockOnTarget->ProjectCAbilitySystemComponent->GetGameplayAttributeValue(UProjectCAttributeSet::GetCurrentHealthAttribute(), bFound);

		if (bFound && CurrentTargetHealth <= 0.f)
		{	
			DrawTargetLockOnWidget(false);
			AvailableTargets.Remove(CurrentLockOnTarget);
			CurrentLockOnTarget = nullptr;
		}
	}
	// 적이 죽으면 다음 적으로 타겟 고정
	if (CurrentLockOnTarget == nullptr && AvailableTargets.Num() > 0)
	{
		CurrentLockOnTarget = Cast<AProjectEnemyCharacter>(UGameplayStatics::FindNearestActor(PlayerLocation, AvailableTargets, CurrentLockOnTargetToDistance));

		DrawTargetLockOnWidget(true);
		if (CurrentLockOnTarget)
		{
			CurrentLockOnTarget->bHealthBarTimeOut = false;
			PlayerOrientRotationToMovement(false);

		}
	}// 마지막 적 죽으면 어빌리티 종료
	else if (CurrentLockOnTarget == nullptr && AvailableTargets.Num() == 0)
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		return;
	}

	// NextLockOnTarget 설정
	AvailableTargets.Remove(CurrentLockOnTarget);
	if (CurrentLockOnTarget && CurrentLockOnTarget != Cast<AProjectEnemyCharacter>(UGameplayStatics::FindNearestActor(PlayerLocation, AvailableTargets, CurrentLockOnTargetToDistance)))
	{
		NextLockOnTarget = Cast<AProjectEnemyCharacter>(UGameplayStatics::FindNearestActor(PlayerLocation, AvailableTargets, CurrentLockOnTargetToDistance));
	}

	GetPlayerCharacterFromActorInfo()->CurrentTarget = CurrentLockOnTarget;
}

void UTargetLockOnAbility::SwitchLockOnTarget()
{
	if (NextLockOnTarget && CurrentLockOnTarget != nullptr && AvailableTargets.Num() > 1)
	{
		// 이전 타겟 UI 설정 초기화
		CurrentLockOnTarget->bHealthBarTimeOut = true;
		DrawTargetLockOnWidget(false);

		CurrentLockOnTarget = NextLockOnTarget;

		// 새로운 타겟 UI 설정
		DrawTargetLockOnWidget(true);
		CurrentLockOnTarget->bHealthBarTimeOut = false;
	}
}

void UTargetLockOnAbility::LockOnTraceLogic(float DeltaTime)
{

	TimeSinceLastTrace += DeltaTime;

	// 특정 시간마다 주변적을 찾음
	if (TimeSinceLastTrace >= TraceInterval)
	{
		AvailableTargets.Empty();
		TimeSinceLastTrace = 0.0f;
		FindLockOnTargetFromTrace();

		// 주변에 적 없으면 자동 취소
		if (AvailableTargets.Num() <= 0)
		{
			CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		}
	}

	// 회전 계속 맞춤
	if (CurrentLockOnTarget && GetPlayerCharacterFromActorInfo())
	{
		APlayerController* PlayerController = CurrentActorInfo->PlayerController.Get();
		if (!PlayerController)
		{
			return;
		}

		FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
		FVector EnemyLocation = CurrentLockOnTarget->GetActorLocation();

		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, EnemyLocation);


		FRotator CurrentRotation = PlayerController->GetControlRotation();

		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, LockOnRotationSpeed);

		PlayerController->SetControlRotation(FRotator(CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll));
	}
	GetPlayerCharacterFromActorInfo()->CurrentTarget = CurrentLockOnTarget;
}

// 주변 적 찾는 트레이스 함수
AProjectEnemyCharacter* UTargetLockOnAbility::FindLockOnTargetFromTrace()
{
	TArray<AActor*> ActorsToIgnore;
	TraceStart = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	TraceEnd = TraceStart;
	TArray<FHitResult> OutHits;
	UKismetSystemLibrary::SphereTraceMultiByProfile
	(
		this,
		TraceStart,
		TraceEnd,
		TraceRadius,
		"Pawn",
		false,
		ActorsToIgnore,
		DrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		OutHits,
		true);

	for (const FHitResult& Hit : OutHits)
	{
		AProjectEnemyCharacter* EnemyCharacter = Cast<AProjectEnemyCharacter>(Hit.GetActor());

		if (CurrentLockOnTarget == EnemyCharacter) continue;

		AvailableTargets.Add(EnemyCharacter);
	}

	return nullptr;
}

void UTargetLockOnAbility::DrawTargetLockOnWidget(bool bDraw)
{
	if (CurrentLockOnTarget)
	{
		CurrentLockOnTarget->TargetLockOnWidget->SetVisibility(bDraw);

		if (!CurrentLockOnTarget->bIsBoss)
			CurrentLockOnTarget->HPWidget->SetVisibility(bDraw);
	}
}

void UTargetLockOnAbility::PlayerOrientRotationToMovement(bool bOrient)
{
	GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->bOrientRotationToMovement = bOrient;
}