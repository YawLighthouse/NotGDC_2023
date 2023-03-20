// Copyright 2023 Nicholas Helish. All rights reserved.


#include "CharacterAbilitiesPrimaryDataAsset.h"

#include "ConfigurationTypes/AbilityConfiguration.h"

#if WITH_EDITORONLY_DATA

void UCharacterAbilitiesPrimaryDataAsset::UpdateBundleDataWithInstancedObjectsMetaData(FAssetBundleData& BundleData)
{
	// Need to have a valid bundle name!
	if(AbilityBundleName.IsNone())
	{
		return;
	}
#if WITH_EDITOR
	for (const UAbilityConfiguration* const Ability : AbilityConfigs)
	{
		CollectSoftPathsFromBlueprints(Ability, AbilityBundleName, BundleData);
	}
#endif
}

#endif

UAbilityConfiguration* UCharacterAbilitiesPrimaryDataAsset::GetFirstAbilityConfig() const
{
	if(!AbilityConfigs.IsValidIndex(0))
	{
		return nullptr;
	}
	return AbilityConfigs[0];
}
