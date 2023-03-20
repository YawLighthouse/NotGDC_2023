// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NotGDC_ConfigurationBase.h"
#include "AbilityConfiguration.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAbilityConfiguration, Log, All);

class ANotGDC_Character;

/**
 * 
 */
UCLASS()
class NOTGDC_2023_API UAbilityConfiguration : public UNotGDC_ConfigurationBase
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool UseAbility(ANotGDC_Character* Character);

	// UObject interface
	virtual UWorld* GetWorld() const override;
	// ~UObject interface

	UFUNCTION(BlueprintPure, Category = "Debugging")
	bool IsDebugging() const { return bDebugging; }
	
protected:	
	
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	bool HandleConfiguration(ANotGDC_Character* Character);
	virtual bool HandleConfiguration_Implementation(ANotGDC_Character* Character) { return true; }
	
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	bool HandleAbilityUse(ANotGDC_Character* Character);
	virtual bool HandleAbilityUse_Implementation(ANotGDC_Character* Character) { return true; }

	UPROPERTY(Transient)
	TObjectPtr<UObject> WorldContext;

	UPROPERTY(EditAnywhere, Category = "Debugging")
	uint8 bDebugging : 1;
	
};
