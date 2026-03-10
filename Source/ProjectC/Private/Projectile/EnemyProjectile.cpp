// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/EnemyProjectile.h"
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

void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCurve)
	{
		if (!ProjectileMovementComponent) return;

		FVector CurrentVel = ProjectileMovementComponent->Velocity;

		if (!CurrentVel.IsNearlyZero())
		{
			// ���� ���� �������� "�� ����(Right)" ���ϱ�
			FVector Right = FVector::CrossProduct(CurrentVel.GetSafeNormal(), FVector::UpVector).GetSafeNormal();

			// ������ ���� �о��ֱ�
			FVector NewVel = CurrentVel + (Right * CurveForce * DeltaTime);

			// �ӵ� ũ��� ���� (�ӵ� �����ϰ�)
			NewVel = NewVel.GetSafeNormal() * CurrentVel.Size();

			ProjectileMovementComponent->Velocity = NewVel;
		}
	}
}


AEnemyProjectile::AEnemyProjectile()
{
}

void AEnemyProjectile::LaunchToTarget()
{
	ProjectileOwner = Cast<AProjectEnemyCharacter>(GetOwner());


	if (ProjectileOwner)
	{
		TargetActor = Cast<AProjectPlayerCharacter>(ProjectileOwner->EnemyAIController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	}

	if (TargetActor)
	{
		FRotator ActorRotation = GetActorRotation();
		FVector DirectionToTarget = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		RotateToTargetFromX = UKismetMathLibrary::MakeRotFromX(DirectionToTarget);
		SetActorRotation(RotateToTargetFromX);

		ProjectileMovementComponent->InitialSpeed = ProjectileInitialSpeed;
		ProjectileMovementComponent->MaxSpeed = ProjectileMaxSpeed;
		ProjectileMovementComponent->Velocity = DirectionToTarget * ProjectileMovementComponent->InitialSpeed;
	}
}

void AEnemyProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// �ڱ� �ڽŰ� �浹�ϴ� ���� �����ϴ� �ڵ�
	if (OtherActor == GetOwner())
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
					//Destroy();
				}
				else // ��� �ƴ� ���
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Front;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

					//Debug::Print("Damage", HitActor->ProjectCAttributeSet->GetCurrentStamina());

					// ���� ȿ�� �����÷��� ť ����
					FGameplayCueParameters Parameters;
					Parameters.Location = HitActor->GetActorLocation();
					HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);
				}
			}
			else if (DotResult < -0.5f) // ������ �ĸ�
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Back;
				//Debug::Print("Back");
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				// ���� ȿ�� �����÷��� ť ����
				FGameplayCueParameters Parameters;
				Parameters.Location = HitActor->GetActorLocation();
				HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);
			}
			else if (Direction > 0.f) // ����
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Right;
				//Debug::Print("Right");
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				// ���� ȿ�� �����÷��� ť ����
				FGameplayCueParameters Parameters;
				Parameters.Location = HitActor->GetActorLocation();
				HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);
			}
			else // ����
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Left;
				//Debug::Print("Left");
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				// ���� ȿ�� �����÷��� ť ����
				FGameplayCueParameters Parameters;
				Parameters.Location = HitActor->GetActorLocation();
				HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);
			}


			// ��ƼŬ �����ϰ� ���� �ı�
			if (Cast<AProjectPlayerCharacter>(HitActor))
				Destroy();
		}
	}
}

void AEnemyProjectile::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemyProjectile::ApplyDamageToHitActors(AActor* InHitActor, float InDamage)
{
	// 자기 자신과 충돌하는 것을 방지하는 코드
	
	if (InHitActor == GetOwner())
	{
		return;
	}

	
	// 데미지 전달
	if (AProjectEnemyCharacter* SourceActor = Cast<AProjectEnemyCharacter>(GetOwner()))
	{
		if (AProjectPlayerCharacter* HitActor = Cast<AProjectPlayerCharacter>(InHitActor))
		{
			UProjectCAbilitySystemComponent* HitActorASC = HitActor->ProjectCAbilitySystemComponent;
			// 시체 딜 방지
			if (HitActorASC->bHealthCheck())
			{
				return;
			}

			// 마법 데미지 받음 태그 전달
			HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Shared_Status_MagicDamageTaken);
			UProjectCAbilitySystemComponent* SourceActorASC = SourceActor->ProjectCAbilitySystemComponent;

			// 데미지 전달하기 위한 컨텍스트, 스펙 생성
			FGameplayEffectContextHandle EffectContext = SourceActorASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = SourceActorASC->MakeOutgoingSpec(DamageEffectClass, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				SpecHandle.Data->SetSetByCallerMagnitude(ProjectCGameplayTags::Data_Damage, InDamage);
			}

			if (HitActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_Invincible))return;

			// 피격 방향 계산
			FVector HitActorToSourceActor = (SourceActor->GetActorLocation() - HitActor->GetActorLocation()).GetSafeNormal(); // 피해자에서 공격자로 가는 방향 벡터
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
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Block;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());


					// 방어 효과 게임플레이 큐 실행
					FGameplayCueParameters Parameters;
					Parameters.Location = HitActor->GetActorLocation();
					HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Block, Parameters);

					return;
				}
				else
				{
					if (bKnockdown)
					{
						HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Front;
					}
					else
					{
						HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Front;
					}
					
					//Debug::Print("Front");
					if (bHitReact)
						UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
			else if (DotResult < -0.5f) // 피해자 후면
			{
				if (bKnockdown)
				{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Back;
				}
				else
				{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Back;
				}
				
				//Debug::Print("Back");
				if (bHitReact)
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
			else if (Direction > 0.f) // 우측
			{
				if (bKnockdown)
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Front;
				}
				else
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Front;
				}
				
				//Debug::Print("Right");
				if (bHitReact)
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
			else // 좌측
			{
				if (bKnockdown)
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Front;
				}
				else
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Front;
				}
				
				//Debug::Print("Left");
				if (bHitReact)
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}

			
			// 게임플레이 큐 실행
			FGameplayCueParameters Parameters;
			Parameters.Location = HitActor->GetActorLocation();
			HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);
			//HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_StarfallHit, Parameters);
		}
	}
}
