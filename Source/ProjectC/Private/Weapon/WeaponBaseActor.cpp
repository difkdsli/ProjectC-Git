// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBaseActor.h"
#include "Components/BoxComponent.h"
#include "Character/ProjectBaseCharacter.h"
#include "Character/ProjectEnemyCharacter.h"
#include "Character/ProjectPlayerCharacter.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ASC/ProjectCGameplayTags.h"
#include "ASC/ProjectCAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "DebugHelper.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBaseActor::AWeaponBaseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	RootComponent = WeaponStaticMesh;
	WeaponStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBoxCollision"));
	WeaponBoxCollision->SetupAttachment(RootComponent);

	WeaponBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
	WeaponBoxCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnComponentEndOverlap);

	WeaponBoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxCollision->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponBoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponBoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponBoxCollision->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AWeaponBaseActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 노티파이 스테이트에서 값 조정하여 공격 온 오프
	if (CollisionEnabled)
	{
		WeaponBoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponBoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeaponBaseActor::DrawDebugTest(FColor Color)
{
	// 디버그 박스가 그려질 월드 내 위치
	const FVector BoxLocation = WeaponBoxCollision->GetComponentLocation();

	// 박스의 크기
	const FVector BoxExtent = WeaponBoxCollision->GetScaledBoxExtent();

	// 박스의 회전값
	const FRotator BoxRotation = WeaponBoxCollision->GetComponentRotation();

	// 올바른 함수 호출
	DrawDebugBox
	(
		GetWorld(),
		BoxLocation, // 
		BoxExtent, // 
		BoxRotation.Quaternion(), //
		Color, // 색상
		false, // 지속성 여부
		2.0f, // 표시 시간
		0, // 우선순위
		2.0f // 두께
	);
}

void AWeaponBaseActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                               const FHitResult& SweepResult)
{
	if (bWeaponHidden) return;

	// 몬스터끼리 팀킬 방지
	if (Cast<AProjectEnemyCharacter>(OtherActor) && Cast<AProjectEnemyCharacter>(GetOwner())) return;

	if (TargetActor.Contains(OtherActor))return;

	if (OtherActor != GetOwner())
	{
		TargetActor.AddUnique(OtherActor);
	}

	if (bDrawDebugCollision)
	{
		DrawDebugTest(FColor::Green);

		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, OtherActor->GetName());
	}

	// 자기 자신과 충돌하는 것을 방지하는 코드
	if (OtherActor == GetOwner())return;

	// 데미지 전달
	if (AProjectBaseCharacter* SourceActor = Cast<AProjectBaseCharacter>(GetOwner()))
	{
		if (AProjectBaseCharacter* HitActor = Cast<AProjectBaseCharacter>(OtherActor))
		{
			// 타겟 죽었는 지 체크
			bool AttrubteFindResult;
			float CurrentHealth = HitActor->ProjectCAbilitySystemComponent->GetGameplayAttributeValue(
				UProjectCAttributeSet::GetCurrentHealthAttribute(), AttrubteFindResult);

			if (AttrubteFindResult && CurrentHealth <= 0)
			{
				return;
			}

			UProjectCAbilitySystemComponent* HitActorASC = HitActor->ProjectCAbilitySystemComponent;

			// 시체 딜 방지
			if (HitActorASC->bHealthCheck())
			{
				return;
			}

			// 물리 데미지 받음 태그 전달
			if (!HitActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_DamageTaken))
			{
				HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Shared_Status_DamageTaken);
			}
			UProjectCAbilitySystemComponent* SourceActorASC = SourceActor->ProjectCAbilitySystemComponent;

			// 데미지 전달하기 위한 컨텍스트, 스펙 생성
			FGameplayEffectContextHandle EffectContext = SourceActorASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = SourceActorASC->
				MakeOutgoingSpec(DamageEffectClass, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				SpecHandle.Data->SetSetByCallerMagnitude(ProjectCGameplayTags::Data_Damage, WeaponDamage);
			}

			// 플레이어 구르기 회피하면 슬로우 및 이펙트효과
			if (HitActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_Invincible))
			{
				if (Cast<AProjectPlayerCharacter>(OtherActor))
				{
					Cast<AProjectPlayerCharacter>(OtherActor)->ActiveDodgeEffect();
				}


				// 시간 슬로우 효과
				FTimerHandle RestoreTimeHandle;
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
				float SlowDuration = 0.5f;
				GetWorld()->GetTimerManager().SetTimer
				(
					RestoreTimeHandle,
					FTimerDelegate::CreateLambda([this,OtherActor]()
						{
							if (GetWorld())
							{
								if (Cast<AProjectPlayerCharacter>(OtherActor))
								{
									Cast<AProjectPlayerCharacter>(OtherActor)->DeactiveDodgeEffect();
								}
								UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
							}
						}
					),
					SlowDuration,
					false
				);


				return;
			}

			// 피격 방향 계산
			FVector HitActorToSourceActor = (SourceActor->GetActorLocation() - HitActor->GetActorLocation()).
				GetSafeNormal(); // 피해자에서 공격자로 가는 방향 벡터
			FVector HitActorForward = HitActor->GetActorForwardVector(); // 피해자의 전방 벡터

			// 피해자의 전방벡터가 공격자로 향하는 전방벡터와 얼마나 일치하는지. 1은 정면 -1은 후면 0은 좌우 둘 중 하나
			float DotResult = FVector::DotProduct(HitActorForward, HitActorToSourceActor);


			// 피해자의 전방벡터와 공격자로 향하는 방향 벡터를 외적하여 나온 값으로 오른쪽 왼쪽 판단
			float Direction = FVector::CrossProduct(HitActorForward, HitActorToSourceActor).Z;


			FGameplayTag HitReactTag;
			FGameplayEventData Payload;
			Payload.Instigator = SourceActor;
			Payload.Target = HitActor;

			if (DotResult > 0.5f) // 피해자 정면
			{
				// 방어 중일 경우
				// 스태미너 1 이상이면 방어 성공
				if (HitActor->ProjectCAttributeSet->GetCurrentStamina() > 0.f && HitActorASC->HasMatchingGameplayTag(
					ProjectCGameplayTags::Shared_Status_Block))
				{
					HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Shared_Status_BlockSuccess);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

					// 방어 성공시 가드 카운터 가능
					if (!HitActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Player_Status_GuardCounterAvailable))
					{
						HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Player_Status_GuardCounterAvailable);
					}
						//HitActorASC->AddLooseGameplayTag(ProjectCGameplayTags::Player_Status_GuardCounterAvailable);

					return;
				} // 패링 하는 경우
				else if (HitActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Player_Status_Parrying))
				{
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						HitActor, ProjectCGameplayTags::Player_Event_ParrySuccess, Payload);
					FGameplayCueParameters Parameters;
					Parameters.Location = HitActor->GetActorLocation();
					HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Block, Parameters);

					// 시간 슬로우 효과
					FTimerHandle RestoreTimeHandle;
					UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
					float SlowDuration = 0.5f;
					GetWorld()->GetTimerManager().SetTimer
					(
						RestoreTimeHandle,
						FTimerDelegate::CreateLambda([this,OtherActor]()
							{
								if (GetWorld())
								{
									if (Cast<AProjectPlayerCharacter>(OtherActor))
									{
										Cast<AProjectPlayerCharacter>(OtherActor)->DeactiveDodgeEffect();
									}
									UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
								}
							}
						),
						SlowDuration,
						false
					);
					return;
				}
				else
				{
					// 데미지 받음
					if (SourceActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_KnockdownOnHit))
					{
						HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Front;
						UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
						HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					}
					else
					{
						HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Front;
						UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
						HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					}
				}
			}
			else if (DotResult < -0.5f) // 피해자 후면
			{
				if (SourceActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_KnockdownOnHit))
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Back;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
				else
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Back;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
			else if (Direction > 0.f) // 우측
			{
				if (SourceActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_KnockdownOnHit))
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Back;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
				else
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Right;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
			else // 좌측
			{
				if (SourceActorASC->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_KnockdownOnHit))
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Knockdown_Back;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
				else
				{
					HitReactTag = ProjectCGameplayTags::Shared_Event_HitReact_Left;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, Payload);
					HitActorASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}

			// 블러드 효과 게임플레이 큐 실행
			FGameplayCueParameters Parameters;
			Parameters.Location = HitActor->GetActorLocation();
			HitActorASC->ExecuteGameplayCue(ProjectCGameplayTags::GameplayCue_VFX_Blood, Parameters);

			//// 적 AI 블랙보드에서 타겟 액터 설정
			if (AProjectEnemyCharacter* EnemyActor = Cast<AProjectEnemyCharacter>(HitActor))
			{
				if (AProjectPlayerCharacter* PlayerOwner = Cast<AProjectPlayerCharacter>(GetOwner()))
				{
					EnemyActor->SetTargetActor(Cast<AProjectPlayerCharacter>(PlayerOwner));
				}
			}
		}
	}
}

void AWeaponBaseActor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 체력 상태 확인하고 화면 테두리에 피격 효과 발생
	if (Cast<AProjectPlayerCharacter>(OtherActor))
	{
		AProjectPlayerCharacter* PlayerCharacter = Cast<AProjectPlayerCharacter>(OtherActor);
		if (PlayerCharacter->ProjectCAttributeSet)
		{
			PlayerCharacter->bIsLowHealth =
				(PlayerCharacter->ProjectCAttributeSet->CurrentHealth.GetCurrentValue() / PlayerCharacter->ProjectCAttributeSet->MaxHealth.GetCurrentValue()) < PlayerCharacter->LowHealthPercent
				&& PlayerCharacter->ProjectCAttributeSet->CurrentHealth.GetCurrentValue() > 0.0f;

			if (PlayerCharacter->bIsLowHealth)
			{
				PlayerCharacter->PlayBloodAnim();
			}
		}
	}

	// 디버그 체크용
	if (bDrawDebugCollision)
	{
		DrawDebugTest(FColor::Red);

		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, OtherActor->GetName());
	}
}
