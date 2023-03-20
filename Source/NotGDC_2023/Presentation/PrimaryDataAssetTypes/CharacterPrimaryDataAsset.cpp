// Copyright 2023 Nicholas Helish. All rights reserved.


#include "CharacterPrimaryDataAsset.h"

TSoftObjectPtr<UCharacterAbilitiesPrimaryDataAsset> UCharacterPrimaryDataAsset::GetFirstDefaultAbility() const
{
	for (TSoftObjectPtr<UCharacterAbilitiesPrimaryDataAsset> AbilityPath : DefaultAbilityPaths)
	{
		if(AbilityPath.IsNull())
		{
			continue;
		}
		return AbilityPath;
	}
	return TSoftObjectPtr<UCharacterAbilitiesPrimaryDataAsset>();
}
