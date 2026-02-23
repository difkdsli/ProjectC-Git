// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASC/PlayerGameplayAbility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TargetLockOnAbility.generated.h"

class AProjectEnemyCharacter;

/**
 * 
 */
UCLASS()
class PROJECTC_API UTargetLockOnAbility : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void LockOnTraceLogic(float DeltaTime);

	UFUNCTION(BlueprintCallable,Category = "LockOn")
	void SetCurrentLockOnTarget(FVector PlayerLocation);

	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void SwitchLockOnTarget();

	UPROPERTY(EditAnywhere)
	float LockOnRotationSpeed{ 10.f };

	UFUNCTION()
	AProjectEnemyCharacter* FindLockOnTargetFromTrace();

	float TimeSinceLastTrace{ 0.f };
	float TraceInterval{ 0.15 };

	UPROPERTY(BlueprintReadWrite)
	FVector TraceStart;
	UPROPERTY(BlueprintReadWrite)
	FVector TraceEnd;
	UPROPERTY(EditAnywhere)
	float TraceRadius{ 100 };

	UPROPERTY(BlueprintReadOnly)
	AProjectEnemyCharacter* CurrentLockOnTarget;
	UPROPERTY(BlueprintReadOnly)
	AProjectEnemyCharacter* NextLockOnTarget;
	UPROPERTY(BlueprintReadWrite)
	float CurrentLockOnTargetToDistance;

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> AvailableTargets;

	UPROPERTY(EditAnywhere)
	bool DrawDebug = false;

	void DrawTargetLockOnWidget(bool bDraw);
	void PlayerOrientRotationToMovement(bool bOrient);

};
