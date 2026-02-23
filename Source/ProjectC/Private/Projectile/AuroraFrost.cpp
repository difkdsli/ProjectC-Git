// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AuroraFrost.h"
#include "Character/ProjectPlayerCharacter.h"
#include "Character/ProjectEnemyCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayEffectTypes.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ASC/ProjectCGameplayTags.h"
#include "ASC/ProjectCAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DebugHelper.h"


AAuroraFrost::AAuroraFrost()
{

}

void AAuroraFrost::BeginPlay()
{
	Super::BeginPlay();
}

void AAuroraFrost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AAuroraFrost::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// 자기 자신과 충돌하는 것을 방지하는 코드
	// 블루프린트에서 오버랩 켜져있는 지 체크 bOverlap
	if (OtherActor == GetOwner())
	{
		return;
	}

	ProjectileOwner = Cast<AProjectEnemyCharacter>(GetOwner());


	if (ProjectileOwner)
	{
		TargetActor = Cast<AProjectPlayerCharacter>(ProjectileOwner->EnemyAIController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	}

	// 데미지 전달
	if (AProjectEnemyCharacter* SourceActor = ProjectileOwner)
	{

		if (AProjectPlayerCharacter* HitActor = Cast<AProjectPlayerCharacter>(OtherActor))
		{
			UProjectCAbilitySystemComponent* HitActorASC = HitActor->ProjectCAbilitySystemComponent;
			// 마법 데미지 받음 태그 전달
			HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Shared_Status_MagicDamageTaken);
			UProjectCAbilitySystemComponent* SourceActorASC = SourceActor->ProjectCAbilitySystemComponent;

			// 데미지 전달하기 위한 컨텍스트, 스펙 생성
			FGameplayEffectContextHandle EffectContext = SourceActorASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = SourceActorASC->MakeOutgoingSpec(DamageEffectClass, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				SpecHandle.Data->SetSetByCallerMagnitude(ProjectCGameplayTags::Data_Damage, ProjectileDamage);
			}

			if (HitActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_Invincible))return;

			// 피격 방향 계산
			//FVector HitActorToSourceActor = (SourceActor->GetActorLocation() - HitActor->GetActorLocation()).GetSafeNormal(); // 피해자에서 공격자로 가는 방향 벡터
			FVector HitActorToSourceActor = (GetActorLocation() - HitActor->GetActorLocation()).GetSafeNormal(); // 피해자에서 공격자로 가는 방향 벡터
			FVector HitActorForward = HitActor->GetActorForwardVector(); // 피해자의 전방 벡터

			float DotResult = FVector::DotProduct(HitActorForward, HitActorToSourceActor); // 피해자의 전방벡터가 공격자로 향하는 전방벡터와 얼마나 일치하는지. 1은 정면 -1은 후면 0은 좌우 둘 중 하나

			float Direction = FVector::CrossProduct(HitActorForward, HitActorToSourceActor).Z; // 피해자의 전방벡터와 공격자로 향하는 방향 벡터를 외적하여 나온 값으로 오른쪽 왼쪽 판단

			FGameplayTag HitReactTag;
			FGameplayEventData Payload;
			Payload.Instigator = SourceActor;
			Payload.Target = HitActor;

			if (DotResult > 0.5f) // 피해자 정면
			{

				// 방어 중일 경우
				// 스태미너 1 이상이면 방어 성공
				if (HitActor->ProjectCAttributeSet->GetCurrentStamina() > 0.f && HitActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_Block))
				{

					HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Shared_Status_BlockSuccess);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					
					// 방어 성공시 가드 카운터 가능
					HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Player_Status_GuardCounterAvailable);
					return;
				}
				else
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Front;
					//Debug::Print("Front");
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				}
			}
			else // 피해자 후면
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Back;
				//Debug::Print("Back");
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}

			// 게임플레이 큐 실행
			FGameplayCueParameters Parameters;
			Parameters.Location = HitActor->GetActorLocation();
			HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);
		}

	
	}
}

void AAuroraFrost::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
