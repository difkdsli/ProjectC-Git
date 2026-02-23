// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTaskTick.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTickDelegate, float, DeltaTime);
/**
 *
 */
UCLASS()
class PROJECTC_API UAbilityTaskTick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTaskTick();
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTaskTick* CreateTickTask(UGameplayAbility* OwningAbility);
	
	UPROPERTY(BlueprintAssignable)
	FTickDelegate OnTick;

	// AbilityTask 기본 Tick 함수 오버라이드
	virtual void TickTask(float DeltaTime) override;

	// Task 시작될 때 호출
	virtual void Activate() override;

	// Task 종료할 때 호출
	virtual void OnDestroy(bool AbilityEnded) override;
};
