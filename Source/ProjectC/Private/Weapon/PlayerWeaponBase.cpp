// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PlayerWeaponBase.h"
#include "ASC/ProjectCAbilitySystemComponent.h"

void APlayerWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorHiddenInGame(bWeaponHidden);
}

void APlayerWeaponBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("APlayerWeaponBase::OnComponentBeginOverlap"));

}

void APlayerWeaponBase::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnComponentEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("APlayerWeaponBase::OnComponentEndOverlap"));

}
