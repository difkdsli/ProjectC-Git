// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ItemDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FItemSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FGameplayTag ItemTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	int DefaultNum{ 0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	int InGameItemNum{ 0 };

	bool IsValid() const
	{
		return ItemImage && ItemTag.IsValid();

	}
};
/**
 *
 */
UCLASS()
class PROJECTC_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion")
	TArray<FItemSet> Potion;
};
