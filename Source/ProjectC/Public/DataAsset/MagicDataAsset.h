// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MagicDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FMagicSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	FGameplayTag MagicTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	UTexture2D* MagicImage;

	bool IsValid() const
	{
		return MagicImage && MagicTag.IsValid();

	}
};
/**
 * 
 */
UCLASS()
class PROJECTC_API UMagicDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
	TArray<FMagicSet> Magic;
};
