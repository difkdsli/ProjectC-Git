// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/ProjectBaseCharacter.h"
#include "Components/SphereComponent.h"
#include "ASC/ProjectCAbilitySystemComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentBeginOverlap);
	CollisionComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentEndOverlap);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Ω√√º µÙ πÊ¡ˆ
	if (AProjectBaseCharacter* HitActor = Cast<AProjectBaseCharacter>(OtherActor))
	{
		UProjectCAbilitySystemComponent* HitActorASC = HitActor->ProjectCAbilitySystemComponent;
		if (HitActorASC->bHealthCheck())
		{
			return;
		}
	}
		
}

void AProjectileBase::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

