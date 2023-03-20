// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ExamplePrimaryDataAsset.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogExamplePrimaryDataAsset, Log, All);

/**
 * An simple example primary data asset for presentation purposes.
 */
UCLASS(ClassGroup = "NotGDC")
class NOTGDC_2023_API UExamplePrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
};
