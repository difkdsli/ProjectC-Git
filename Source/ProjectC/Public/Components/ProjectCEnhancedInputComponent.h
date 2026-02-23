// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "DataAsset/InputDataAsset.h"

#include "ProjectCEnhancedInputComponent.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTC_API UProjectCEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	

public:
	template<class UserObject, typename CallbackFunc>
	inline void NativeBind(UInputDataAsset* InInputDataAsset,const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* Object, CallbackFunc FunctionName);

	template<class UserObject, typename CallbackFunc>
	inline void AbilityBind(UInputDataAsset* InInputDataAsset,UserObject* Object, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);

};

template<class UserObject, typename CallbackFunc>
inline void UProjectCEnhancedInputComponent::NativeBind(UInputDataAsset* InInputDataAsset, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* Object, CallbackFunc FunctionName)
{
	for (FInputData InputData : InInputDataAsset->NaitveInputAction)
	{
		if (InputData.InputTag == InInputTag)
		{
			BindAction(InputData.InputAction, TriggerEvent, Object, FunctionName);

		}
	}
}

//inline void UProjectCEnhancedInputComponent::AbilityBind(UInputDataAsset* InInputDataAsset, const FGameplayTag& InInputTag, UserObject* Object, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
template<class UserObject, typename CallbackFunc>
inline void UProjectCEnhancedInputComponent::AbilityBind(UInputDataAsset* InInputDataAsset, UserObject* Object, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	for (FInputData InputData : InInputDataAsset->AbilityInputAction)
	{
		//if (InputData.InputTag == InInputTag)
		if (InputData.InputTag.IsValid())
		{
			BindAction(InputData.InputAction, ETriggerEvent::Started, Object, InputPressedFunc,InputData.InputTag);
			BindAction(InputData.InputAction, ETriggerEvent::Completed, Object, InputReleasedFunc,InputData.InputTag);

		}
	}
}


