// Copyright 2023 Nicholas Helish. All rights reserved.

#include "AbilityConfiguration.h"
#include "NotGDC_Character.h"
#include "NotGDC_Logging.h"

DEFINE_LOG_CATEGORY(LogAbilityConfiguration);

bool UAbilityConfiguration::UseAbility(ANotGDC_Character* Character)
{
	if(!IsValid(Character))
	{
		LOG_SCREEN_NOTGDC(LogAbilityConfiguration, 5.0f, Error, "Inputted NULL Character to UAbilityConfiguration(%s)", *GetName());
		return false;
	}
	// Set our context for use while the ability is active
	WorldContext = Character;
	if(!HandleConfiguration(Character))
	{
		LOG_NOTGDC(LogAbilityConfiguration, Warning, "Failed HandleConfiguration for Character(%s) on UAbilityConfiguration(%s)",
			*GetNameSafe(Character), *GetName());
		return false;
	}
	if(!HandleAbilityUse(Character))
	{
		LOG_NOTGDC(LogAbilityConfiguration, Warning, "Failed HandleAbilityUse for Character(%s) on UAbilityConfiguration(%s)",
			*GetNameSafe(Character), *GetName());
		return false;
	}
	// Reset it to null for the next activation
	WorldContext = nullptr;
	return true; 
}

UWorld* UAbilityConfiguration::GetWorld() const
{
	if(!IsValid(WorldContext))
	{
		return nullptr;
	}
	return WorldContext->GetWorld();
}
