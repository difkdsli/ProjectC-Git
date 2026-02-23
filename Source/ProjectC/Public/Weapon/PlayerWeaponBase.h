// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBaseActor.h"
#include "PlayerWeaponBase.generated.h"

/**
 *
 */
UCLASS()
class PROJECTC_API APlayerWeaponBase : public AWeaponBaseActor
{
	GENERATED_BODY()


public:

	virtual void Tick(float DeltaTime) override;

public:

	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

	virtual void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* CurrentWeaponImage;

	
};
