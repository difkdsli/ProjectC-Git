// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/ProjectileBase.h"
#include "EnemyProjectile.generated.h"

class UGameplayEffect;
class AProjectPlayerCharacter;
class AProjectEnemyCharacter;
/**
 *
 */
UCLASS()
class PROJECTC_API AEnemyProjectile : public AProjectileBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:

	AEnemyProjectile();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void LaunchToTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileInitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileDamage{ 10 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	AProjectPlayerCharacter* TargetActor;

	UPROPERTY()
	AProjectEnemyCharacter* ProjectileOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	FRotator RotateToTargetFromX = FRotator::ZeroRotator;

	/*한쪽으로 휘게 하는 힘 (값 클수록 급격히 휨)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float CurveForce{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool bCurve = false;

	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
