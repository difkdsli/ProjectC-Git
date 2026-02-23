// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ProjectCBaseAnimInstance.generated.h"

class AProjectBaseCharacter;
class UProjectCAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class PROJECTC_API UProjectCBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation()override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds)override;

	virtual void NativeUpdateAnimation(float DeltaSeconds)override;
	
	virtual void NativePostEvaluateAnimation()override; // 애니메이션 끝나고 호출

public:

	UPROPERTY(BlueprintReadOnly, Category = "CharacterData")
	UProjectCAbilitySystemComponent* OwnerAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterData")
	TObjectPtr<AProjectBaseCharacter> OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterData")
	float GroundSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterData")
	float Angle;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterData")
	bool bHasBlockTag = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterData")
	bool bIsFlying = false;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterData")
	bool bHasUpperBodyTag = false;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterData")
	bool bHasAtGraceTag = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterData")
	bool bIsFalling = false;
};
