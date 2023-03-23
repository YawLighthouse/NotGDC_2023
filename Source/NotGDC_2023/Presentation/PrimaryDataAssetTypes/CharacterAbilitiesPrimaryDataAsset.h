// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NotGDC_PrimaryDataAsset.h"
#include "CharacterAbilitiesPrimaryDataAsset.generated.h"

class UAbilityConfiguration;

/**
 * 
 */
UCLASS()
class NOTGDC_2023_API UCharacterAbilitiesPrimaryDataAsset : public UNotGDC_PrimaryDataAsset
{
	GENERATED_BODY()
public:

#if WITH_EDITORONLY_DATA
	
	// UNotGDC_PrimaryDataAsset interface
	virtual void UpdateBundleDataWithInstancedObjectsMetaData(FAssetBundleData& BundleData) override;
	// ~UNotGDC_PrimaryDataAsset interface

#endif
	
	UFUNCTION(BlueprintPure, Category = "Abilities")
	UAbilityConfiguration* GetFirstAbilityConfig() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FText DisplayName = FText::FromString("DEFAULT ABILITY NAME");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI",
		meta = (DisplayThumbnail="true", AssetBundles = "UI",
			AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface",
			DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	TSoftObjectPtr<UObject> Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	FName AbilityBundleName = "Abilities";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Abilities")
	TArray<TObjectPtr<UAbilityConfiguration>> AbilityConfigs;
	
};
