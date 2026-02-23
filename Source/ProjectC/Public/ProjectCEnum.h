// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EDodgeDirection : uint8
{
    Forward        UMETA(DisplayName = "Forward"),
    Backward       UMETA(DisplayName = "Backward"),
    Left           UMETA(DisplayName = "Left"),
    Right          UMETA(DisplayName = "Right"),
    ForwardLeft    UMETA(DisplayName = "Forward Left"),
    ForwardRight   UMETA(DisplayName = "Forward Right"),
    BackwardLeft   UMETA(DisplayName = "Backward Left"),
    BackwardRight  UMETA(DisplayName = "Backward Right"),
    None           UMETA(DisplayName = "None")
};

