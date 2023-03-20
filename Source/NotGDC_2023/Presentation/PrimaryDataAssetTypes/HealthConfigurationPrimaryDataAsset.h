// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NotGDC_PrimaryDataAsset.h"
#include "HealthConfigurationPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class NOTGDC_2023_API UHealthConfigurationPrimaryDataAsset : public UNotGDC_PrimaryDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DefaultHealth = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ForceInlineRow, AssetBundles = "DamageTypes"))
	TMap<TSoftClassPtr<UDamageType>, float> DamageMultipliers;
	
};
