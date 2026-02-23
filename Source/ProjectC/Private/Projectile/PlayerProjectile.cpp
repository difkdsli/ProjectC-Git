// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/PlayerProjectile.h"
#include "Character/ProjectPlayerCharacter.h"
#include "Character/ProjectEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ASC/ProjectCGameplayTags.h"
#include "ASC/ProjectCAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"
#include "Weapon/WeaponBaseActor.h"

#include "DebugHelper.h"

APlayerProjectile::APlayerProjectile()
{

}

void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->InitialSpeed = 0.f;
	ProjectileMovementComponent->MaxSpeed = 0.f;
	ProjectileMovementComponent->bIsHomingProjectile = bOnHoming;
	ProjectileMovementComponent->HomingAccelerationMagnitude = HomingAccel;

	FTimerHandle SetTargetTimerHandle;
	FTimerHandle LaunchTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(SetTargetTimerHandle, this, &ThisClass::SetTarget, 0.1f, true);

	GetWorld()->GetTimerManager().SetTimer(LaunchTimerHandle, this, &ThisClass::LaunchToTarget, LaunchStartTime, false);

}

void APlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target)
	{
		//if (bOnHoming)
		//{
		FVector DirectionToTarget = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		RotateToTarget = UKismetMathLibrary::MakeRotFromX(DirectionToTarget);

		//}

		if (!bLaunch)
		{
			FRotator Rot = FMath::RInterpTo(GetActorRotation(), RotateToTarget, DeltaTime, 10.0f);
			SetActorRotation(Rot);

		}

	}

}

void APlayerProjectile::SetTarget()
{
	if (Cast<AProjectPlayerCharacter>(GetOwner()))
	{
		AProjectPlayerCharacter* ProjectileOwner = Cast<AProjectPlayerCharacter>(GetOwner());
		Target = ProjectileOwner->CurrentTarget;

		if (Target && bOnHoming)
		{
			ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
		}
	}
}

void APlayerProjectile::LaunchToTarget()
{
	if (Target)
	{
		FVector DirectionToTarget = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		RotateToTarget = UKismetMathLibrary::MakeRotFromX(DirectionToTarget);

		ProjectileMovementComponent->InitialSpeed = LaunchInitialSpeed;
		ProjectileMovementComponent->MaxSpeed = LaunchMaxSpeed;
		ProjectileMovementComponent->Velocity = DirectionToTarget * ProjectileMovementComponent->InitialSpeed;
		bLaunch = true;
	}
	else
	{
		FVector DirectionToTarget = ((GetActorForwardVector() * 300.f + GetActorLocation()) - GetActorLocation()).GetSafeNormal();
		RotateToTarget = UKismetMathLibrary::MakeRotFromX(DirectionToTarget);

		ProjectileMovementComponent->InitialSpeed = LaunchInitialSpeed;
		ProjectileMovementComponent->MaxSpeed = LaunchMaxSpeed;
		ProjectileMovementComponent->Velocity = DirectionToTarget * ProjectileMovementComponent->InitialSpeed;

	}
}

void APlayerProjectile::ApplyDamageToHitActors(AActor* InHitActor,float InDamage)
{
	// 자기 자신과 충돌하는 것을 방지하는 코드
	if (InHitActor == GetOwner())
	{
		return;
	}

	// 데미지 전달
	if (AProjectPlayerCharacter* SourceActor = Cast<AProjectPlayerCharacter>(GetOwner()))
	{

		if (AProjectEnemyCharacter* HitActor = Cast<AProjectEnemyCharacter>(InHitActor))
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
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Front;
					//Debug::Print("Front");
					if (bHitReact)
						UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				}
			}
			else if (DotResult < -0.5f) // 피해자 후면
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Back;
				//Debug::Print("Back");
				if (bHitReact)
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
			else if (Direction > 0.f) // 우측
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Right;
				//Debug::Print("Right");
				if (bHitReact)
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
			else // 좌측
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Left;
				//Debug::Print("Left");
				if (bHitReact)
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}


			// 적 AI 블랙보드에서 타겟 액터 설정
			if (AProjectEnemyCharacter* EnemyActor = Cast<AProjectEnemyCharacter>(HitActor))
			{
				if (AProjectPlayerCharacter* PlayerOwner = Cast<AProjectPlayerCharacter>(GetOwner()))
				{
					EnemyActor->SetTargetActor(Cast<AProjectPlayerCharacter>(PlayerOwner));
				}
			}

			// 게임플레이 큐 실행
			FGameplayCueParameters Parameters;
			Parameters.Location = HitActor->GetActorLocation();
			HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);
			HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_StarfallHit, Parameters);

		}

	}
}

void APlayerProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// 자기 자신과 충돌하는 것을 방지하는 코드
	if (OtherActor == GetOwner())
	{
		return;
	}


	// 데미지 전달
	if (AProjectPlayerCharacter* SourceActor = Cast<AProjectPlayerCharacter>(GetOwner()))
	{

		if (AProjectEnemyCharacter* HitActor = Cast<AProjectEnemyCharacter>(OtherActor))
		{
			UProjectCAbilitySystemComponent* HitActorASC = HitActor->ProjectCAbilitySystemComponent;

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
				SpecHandle.Data->SetSetByCallerMagnitude(ProjectCGameplayTags::Data_Damage, SourceActor->CurrentEquipStaff->WeaponDamage);
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
					Destroy();

					// 방어 효과 게임플레이 큐 실행
					FGameplayCueParameters Parameters;
					Parameters.Location = HitActor->GetActorLocation();
					HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Block, Parameters);

					return;
				}
				else
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Front;
					//Debug::Print("Front");
					if(bHitReact)
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				}
			}
			else if (DotResult < -0.5f) // 피해자 후면
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Back;
				//Debug::Print("Back");
				if (bHitReact)
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
			else if (Direction > 0.f) // 우측
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Right;
				//Debug::Print("Right");
				if (bHitReact)
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
			else // 좌측
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Left;
				//Debug::Print("Left");
				if (bHitReact)
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}


			// 적 AI 블랙보드에서 타겟 액터 설정
			if (AProjectEnemyCharacter* EnemyActor = Cast<AProjectEnemyCharacter>(HitActor))
			{
				if (AProjectPlayerCharacter* PlayerOwner = Cast<AProjectPlayerCharacter>(GetOwner()))
				{
					EnemyActor->SetTargetActor(Cast<AProjectPlayerCharacter>(PlayerOwner));
				}
			}

			// 게임플레이 큐 실행
			FGameplayCueParameters Parameters;
			Parameters.Location = HitActor->GetActorLocation();
			HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);
			
			// 파티클 실행하고 액터 파괴
			if (Cast<AProjectEnemyCharacter>(HitActor))
				Destroy();
		}

	}

}

void APlayerProjectile::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnComponentEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

