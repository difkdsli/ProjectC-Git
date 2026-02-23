// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/EnemyProjectile.h"
#include "AuroraPhantomRushProjectile.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTC_API AAuroraPhantomRushProjectile : public AEnemyProjectile
{
	GENERATED_BODY()


protected:

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bOverlap = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bDistance = false;

	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
