// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ProjectEnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "DebugHelper.h"
#include "ASC/ProjectCGameplayTags.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ASC/ProjectCAttributeSet.h"
#include "Widget/BaseUserWidget.h"
#include "Widget/EnemyUserWidget.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"


AProjectEnemyCharacter::AProjectEnemyCharacter()
{
	TargetLockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetLockOnWiget"));
	TargetLockOnWidget->SetupAttachment(RootComponent);

	HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));
	HPWidget->SetupAttachment(RootComponent);

	HPWidget->SetVisibility(false);
	TargetLockOnWidget->SetVisibility(false);

}

void AProjectEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 1. 에디터에서 설정한 '설계도(HPWidgetClass)'가 있는지 확인
	if (HPWidgetClass)
	{
		// 2. '설계도'를 바탕으로 실제 위젯 객체(인스턴스)를 생성
		HPUserWidget = CreateWidget<UBaseUserWidget>(GetWorld(), HPWidgetClass);

		// 3. 생성된 위젯 객체가 유효한지 확인
		if (HPUserWidget)
		{
			if (bIsBoss)
			{
				HPWidget->SetWidget(HPUserWidget);
			}
			else
			{
				// 4. 위젯 컴포넌트에 방금 생성한 위젯을 붙여줌
				HPWidget->SetWidget(HPUserWidget);
				HPWidget->SetVisibility(false);

			}

			HPUserWidget->UIOwnerASC = ProjectCAbilitySystemComponent;

			// 5. AttributeSet이 유효한지 확인 (부모 클래스에서 생성되었으므로 유효함)
			if (ProjectCAttributeSet)
			{
				// 6. AttributeSet에서 초기값을 가져와 위젯 변수에 할당
				HPUserWidget->MaxHealth = ProjectCAttributeSet->GetMaxHealth();
				HPUserWidget->CurrentHealth = ProjectCAttributeSet->GetCurrentHealth();
			}
		}
	}


	EnemyAIController = Cast<AEnemyAIController>(GetController());

	FTimerHandle TargetActorCheckTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TargetActorCheckTimerHandle, this, &ThisClass::CheckTargetActor, 1.0f, true);
}

void AProjectEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HPWidget && bHealthBarTimeOut)
	{
		TimeSinceLastHit += DeltaTime;

		if (TimeSinceLastHit >= HealthBarTimeout)
		{
			HPWidget->SetVisibility(false);
			TimeSinceLastHit = 0;
		}
	}

	// 죽으면 락온 위젯 제거
	bool bFound;
	if (ProjectCAbilitySystemComponent && TargetLockOnWidget)
	{
		float CurrentHealth = ProjectCAbilitySystemComponent->GetGameplayAttributeValue(UProjectCAttributeSet::GetCurrentHealthAttribute(), bFound);

		if (bFound && CurrentHealth <= 0.f)
		{
			TargetLockOnWidget->SetVisibility(false);

		}
	}
}

void AProjectEnemyCharacter::SetTargetActor(UObject* Target)
{
	if (EnemyAIController && EnemyAIController->GetBlackboardComponent())
	{

		EnemyAIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", Target);

	}
}

void AProjectEnemyCharacter::CheckTargetActor()
{
	if (EnemyAIController && EnemyAIController->GetBlackboardComponent())
	{
		UBlackboardComponent* EnemyBlackboard = EnemyAIController->GetBlackboardComponent();

		// 뒤잡 설정
		if (!EnemyBlackboard->GetValueAsObject("TargetActor"))
		{
			if (!ProjectCAbilitySystemComponent->HasMatchingGameplayTag(ProjectCGameplayTags::Enemy_Status_ExecutionAvailable))
			{
				ProjectCAbilitySystemComponent->AddLooseGameplayTag(ProjectCGameplayTags::Enemy_Status_ExecutionAvailable);
			}
		} // 처형 게이지 0 이면 처형 가능 태그 제거
		else if (EnemyBlackboard && ProjectCAttributeSet && EnemyBlackboard->GetValueAsObject("TargetActor") && ProjectCAttributeSet->GetExecutionGauge() == 0)
		{
			if (ProjectCAbilitySystemComponent->HasMatchingGameplayTag(ProjectCGameplayTags::Enemy_Status_ExecutionAvailable))
			{
				ProjectCAbilitySystemComponent->RemoveLooseGameplayTag(ProjectCGameplayTags::Enemy_Status_ExecutionAvailable);
			}

		}
		else
		{
			return;
		}
	}

}

void AProjectEnemyCharacter::OnAnyAttributeChanged(const FOnAttributeChangeData& Data)
{
	// 피격 시 다시 카운트
	if (!bIsBoss) // 보스일 경우 몹 머리 위 체력바 안보임
	{
		HPWidget->SetVisibility(true);
		TimeSinceLastHit = 0;

	}
	//

	if (HPWidget && HPWidget->GetUserWidgetObject() && Data.Attribute == UProjectCAttributeSet::GetCurrentHealthAttribute())
	{
		if (UBaseUserWidget* Widget = Cast<UBaseUserWidget>(HPWidget->GetUserWidgetObject()))
		{
			Widget->CurrentHealth = Data.NewValue;
		}
	}
	if (HPWidget && HPWidget->GetUserWidgetObject() && Data.Attribute == UProjectCAttributeSet::GetMaxHealthAttribute())
	{
		if (UBaseUserWidget* Widget = Cast<UBaseUserWidget>(HPWidget->GetUserWidgetObject()))
		{
			Widget->MaxHealth = Data.NewValue;
		}
	}


}
