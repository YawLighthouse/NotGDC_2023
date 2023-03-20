// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NotGDC_ConfigurationBase.generated.h"

/**
 * The base configuration class for all configuration types in the project.
 */
UCLASS(ClassGroup = "NotGDC", HideDropdown, EditInlineNew, DefaultToInstanced, Blueprintable)
class NOTGDC_2023_API UNotGDC_ConfigurationBase : public UObject
{
	GENERATED_BODY()
};

/**
 * A container for blueprint to be able to setup configuration objects without more C++ support. 
 */
USTRUCT(BlueprintType)
struct NOTGDC_2023_API FNotGDC_ConfigurationContainer
{
	GENERATED_BODY()
public:

	FNotGDC_ConfigurationContainer()
	{ }

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TObjectPtr<UNotGDC_ConfigurationBase> ConfigurationObject;
	
};
