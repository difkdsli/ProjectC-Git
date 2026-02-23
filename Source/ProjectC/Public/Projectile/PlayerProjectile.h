// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/ProjectileBase.h"
#include "GameplayTagContainer.h"
#include "PlayerProjectile.generated.h"

class AProjectEnemyCharacter;
class UGameplayEffect;

/**
 *
 */
UCLASS()
class PROJECTC_API APlayerProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	APlayerProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintReadOnly)
	AProjectEnemyCharacter* Target;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	FRotator RotateToTarget = FRotator::ZeroRotator;

	void SetTarget();

	void LaunchToTarget();

	UFUNCTION(BlueprintCallable)
	void ApplyDamageToHitActors(AActor* InHitActor,float InDamage);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	bool bOnHoming = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	bool bLaunch = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	bool bHitReact = false;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float HomingAccel = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float LaunchInitialSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float LaunchMaxSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float LaunchStartTime = 0.1f;



	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
