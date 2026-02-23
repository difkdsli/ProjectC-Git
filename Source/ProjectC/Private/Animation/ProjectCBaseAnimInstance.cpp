// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ProjectCBaseAnimInstance.h"
#include "Character/ProjectBaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "ASC/ProjectCGameplayTags.h"
#include "ASC/ProjectCAbilitySystemComponent.h"
#include "ASC/ProjectCGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"

void UProjectCBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (APawn* Pawn = TryGetPawnOwner())
	{
		OwnerCharacter = Cast<AProjectBaseCharacter>(Pawn);
		if (OwnerCharacter)
		{
			OwnerAbilitySystemComponent = Cast<UProjectCAbilitySystemComponent>(OwnerCharacter->GetAbilitySystemComponent());
		}
	}
}

void UProjectCBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter)
	{
		if (OwnerAbilitySystemComponent)
		{
			bHasUpperBodyTag = OwnerAbilitySystemComponent->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Layer_UpperBody);
			bHasAtGraceTag = OwnerAbilitySystemComponent->HasMatchingGameplayTag(ProjectCGameplayTags::Player_Status_AtGrace);
			bIsFalling = OwnerAbilitySystemComponent->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_InAir);
		}

		//Angle = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity()), OwnerCharacter->GetActorRotation()).Yaw;
		//float TargetAngle = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity()), OwnerCharacter->GetActorRotation()).Yaw;

		GroundSpeed = OwnerCharacter->GetVelocity().Size();
		Angle = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetActorRotation());
		bHasBlockTag = OwnerCharacter->ProjectCAbilitySystemComponent->HasMatchingGameplayTag(ProjectCGameplayTags::Shared_Status_Block);
		bIsFlying = OwnerCharacter->GetCharacterMovement()->IsFlying();

	}


}

void UProjectCBaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);



}

void UProjectCBaseAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

