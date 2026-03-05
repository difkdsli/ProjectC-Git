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
	// �ڱ� �ڽŰ� �浹�ϴ� ���� �����ϴ� �ڵ�
	if (InHitActor == GetOwner())
	{
		return;
	}

	// ������ ����
	if (AProjectPlayerCharacter* SourceActor = Cast<AProjectPlayerCharacter>(GetOwner()))
	{

		if (AProjectEnemyCharacter* HitActor = Cast<AProjectEnemyCharacter>(InHitActor))
		{
			UProjectCAbilitySystemComponent* HitActorASC = HitActor->ProjectCAbilitySystemComponent;

			// ��ü �� ����
			if (HitActorASC->bHealthCheck())
			{
				return;
			}

			// ���� ������ ���� �±� ����
			HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Shared_Status_MagicDamageTaken);
			UProjectCAbilitySystemComponent* SourceActorASC = SourceActor->ProjectCAbilitySystemComponent;

			// ������ �����ϱ� ���� ���ؽ�Ʈ, ���� ����
			FGameplayEffectContextHandle EffectContext = SourceActorASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = SourceActorASC->MakeOutgoingSpec(DamageEffectClass, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				SpecHandle.Data->SetSetByCallerMagnitude(ProjectCGameplayTags::Data_Damage, InDamage);
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
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Block;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				

					// ��� ȿ�� �����÷��� ť ����
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
			else if (DotResult < -0.5f) // ������ �ĸ�
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Back;
				//Debug::Print("Back");
				if (bHitReact)
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
			else if (Direction > 0.f) // ����
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Right;
				//Debug::Print("Right");
				if (bHitReact)
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
			else // ����
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Left;
				//Debug::Print("Left");
				if (bHitReact)
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}


			// �� AI �������忡�� Ÿ�� ���� ����
			if (AProjectEnemyCharacter* EnemyActor = Cast<AProjectEnemyCharacter>(HitActor))
			{
				if (AProjectPlayerCharacter* PlayerOwner = Cast<AProjectPlayerCharacter>(GetOwner()))
				{
					EnemyActor->SetTargetActor(Cast<AProjectPlayerCharacter>(PlayerOwner));
				}
			}

			// �����÷��� ť ����
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

	// �ڱ� �ڽŰ� �浹�ϴ� ���� �����ϴ� �ڵ�
	if (OtherActor == GetOwner())
	{
		return;
	}


	// ������ ����
	if (AProjectPlayerCharacter* SourceActor = Cast<AProjectPlayerCharacter>(GetOwner()))
	{

		if (AProjectEnemyCharacter* HitActor = Cast<AProjectEnemyCharacter>(OtherActor))
		{
			UProjectCAbilitySystemComponent* HitActorASC = HitActor->ProjectCAbilitySystemComponent;

			if (HitActorASC->bHealthCheck())
			{
				return;
			}

			// ���� ������ ���� �±� ����
			HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Shared_Status_MagicDamageTaken);
			UProjectCAbilitySystemComponent* SourceActorASC = SourceActor->ProjectCAbilitySystemComponent;

			// ������ �����ϱ� ���� ���ؽ�Ʈ, ���� ����
			FGameplayEffectContextHandle EffectContext = SourceActorASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = SourceActorASC->MakeOutgoingSpec(DamageEffectClass, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				SpecHandle.Data->SetSetByCallerMagnitude(ProjectCGameplayTags::Data_Damage, SourceActor->CurrentEquipStaff->WeaponDamage);
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
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Block;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					Destroy();

					// ��� ȿ�� �����÷��� ť ����
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
			else if (DotResult < -0.5f) // ������ �ĸ�
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Back;
				//Debug::Print("Back");
				if (bHitReact)
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
			else if (Direction > 0.f) // ����
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Right;
				//Debug::Print("Right");
				if (bHitReact)
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
			else // ����
			{
				HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Left;
				//Debug::Print("Left");
				if (bHitReact)
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
				HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}


			// �� AI �������忡�� Ÿ�� ���� ����
			if (AProjectEnemyCharacter* EnemyActor = Cast<AProjectEnemyCharacter>(HitActor))
			{
				if (AProjectPlayerCharacter* PlayerOwner = Cast<AProjectPlayerCharacter>(GetOwner()))
				{
					EnemyActor->SetTargetActor(Cast<AProjectPlayerCharacter>(PlayerOwner));
				}
			}

			// �����÷��� ť ����
			FGameplayCueParameters Parameters;
			Parameters.Location = HitActor->GetActorLocation();
			HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);
			
			// ��ƼŬ �����ϰ� ���� �ı�
			if (Cast<AProjectEnemyCharacter>(HitActor))
				Destroy();
		}

	}

}

void APlayerProjectile::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnComponentEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

