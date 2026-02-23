#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

namespace Debug
{
	// 문자열만 출력
	inline void Print(const FString& Message, FColor Color = FColor::Green, float Duration = 2.f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
		}
	}

	// 숫자(float) 출력
	inline void Print(float Value, FColor Color = FColor::Green, float Duration = 2.f)
	{
		Print(FString::SanitizeFloat(Value), Color, Duration);
	}

	// 숫자(int32) 출력
	inline void Print(int32 Value, FColor Color = FColor::Green, float Duration = 2.f)
	{
		Print(FString::FromInt(Value), Color, Duration);
	}
	// 숫자(float), 문자열 출력
	inline void Print(const FString& Message, float Value, FColor Color = FColor::Green, float Duration = 2.f)
	{
		if (GEngine)
		{
			FString FloatValue = FString::Printf(TEXT("%f"), Value);
			GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message + FloatValue);
		}
	}
}