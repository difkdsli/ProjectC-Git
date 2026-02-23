// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/AT/AbilityTaskTick.h"

UAbilityTaskTick::UAbilityTaskTick()
{
	bTickingTask = true;
}

UAbilityTaskTick* UAbilityTaskTick::CreateTickTask(UGameplayAbility* OwningAbility)
{
	
	UAbilityTaskTick* TickTask = NewAbilityTask<UAbilityTaskTick>(OwningAbility);
	return TickTask;
}

void UAbilityTaskTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	OnTick.Broadcast(DeltaTime);
}

void UAbilityTaskTick::Activate()
{
	Super::Activate();
	
}

void UAbilityTaskTick::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}
