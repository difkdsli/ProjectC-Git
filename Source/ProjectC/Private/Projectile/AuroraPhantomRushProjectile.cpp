// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AuroraPhantomRushProjectile.h"
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

void AAuroraPhantomRushProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AAuroraPhantomRushProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(TargetActor)
	bDistance = 100 <= FVector::Distance(GetActorLocation(), TargetActor->GetActorLocation());
}

void AAuroraPhantomRushProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	// �ڱ� �ڽŰ� �浹�ϴ� ���� �����ϴ� �ڵ�
	// �������Ʈ���� ������ �����ִ� �� üũ bOverlap
	if (OtherActor == GetOwner() || !bOverlap)
	{
		return;
	}



	// ������ ����
	if (AProjectEnemyCharacter* SourceActor = ProjectileOwner)
	{

		if (AProjectPlayerCharacter* HitActor = Cast<AProjectPlayerCharacter>(OtherActor))
		{
			UProjectCAbilitySystemComponent* HitActorASC = HitActor->ProjectCAbilitySystemComponent;
			// ���� ������ ���� �±� ����
			HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Shared_Status_MagicDamageTaken);
			UProjectCAbilitySystemComponent* SourceActorASC = SourceActor->ProjectCAbilitySystemComponent;

			// ������ �����ϱ� ���� ���ؽ�Ʈ, ���� ����
			FGameplayEffectContextHandle EffectContext = SourceActorASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = SourceActorASC->MakeOutgoingSpec(DamageEffectClass, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				SpecHandle.Data->SetSetByCallerMagnitude(ProjectCGameplayTags::Data_Damage, ProjectileDamage);
			}

			if (HitActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_Invincible))return;

			// �ǰ� ���� ���
			FVector HitActorToSourceActor = (SourceActor->GetActorLocation() - HitActor->GetActorLocation()).GetSafeNormal(); // �����ڿ��� �����ڷ� ���� ���� ����
			FVector HitActorForward = HitActor->GetActorForwardVector(); // �������� ���� ����

			float DotResult = FVector::DotProduct(HitActorForward, HitActorToSourceActor); // �������� ���溤�Ͱ� �����ڷ� ���ϴ� ���溤�Ϳ� �󸶳� ��ġ�ϴ���. 1�� ���� -1�� �ĸ� 0�� �¿� �� �� �ϳ�

			float Direction = FVector::CrossProduct(HitActorForward, HitActorToSourceActor).Z; // �������� ���溤�Ϳ� �����ڷ� ���ϴ� ���� ���͸� �����Ͽ� ���� ������ ������ ���� �Ǵ�

			FGameplayTag HitReactTag;
			FGameplayEventData Payload;
			Payload.Instigator = SourceActor;
			Payload.Target = HitActor;

			if (DotResult > 0.5f) // ������ ����
			{

				// ��� ���� ���
				// ���¹̳� 1 �̻��̸� ��� ����
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
			else // ������ �ĸ�
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Back;
				//Debug::Print("Back");
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}

			// �����÷��� ť ����
			FGameplayCueParameters Parameters;
			Parameters.Location = HitActor->GetActorLocation();
			HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);
		}

	}
}

void AAuroraPhantomRushProjectile::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
