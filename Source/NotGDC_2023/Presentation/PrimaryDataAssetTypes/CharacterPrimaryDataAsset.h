// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NotGDC_PrimaryDataAsset.h"
#include "CharacterPrimaryDataAsset.generated.h"

class UCharacterAbilitiesPrimaryDataAsset;

/**
 * 
 */
UCLASS()
class NOTGDC_2023_API UCharacterPrimaryDataAsset : public UNotGDC_PrimaryDataAsset
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "Abilities")
	TSoftObjectPtr<UCharacterAbilitiesPrimaryDataAsset> GetFirstDefaultAbility() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AssetBundles = "Abilities"))
	TSoftObjectPtr<UCharacterAbilitiesPrimaryDataAsset> JumpAbilityPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AssetBundles = "Abilities"))
	TSet<TSoftObjectPtr<UCharacterAbilitiesPrimaryDataAsset>> DefaultAbilityPaths;
	
};
