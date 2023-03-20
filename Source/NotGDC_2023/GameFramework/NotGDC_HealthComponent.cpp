// Copyright 2023 Nicholas Helish. All rights reserved.


#include "NotGDC_HealthComponent.h"

#include "NotGDC_Logging.h"
#include "Engine/AssetManager.h"
#include "PrimaryDataAssetTypes/HealthConfigurationPrimaryDataAsset.h"

DEFINE_LOG_CATEGORY(LogHealthComponent);

// Sets default values for this component's properties
UNotGDC_HealthComponent::UNotGDC_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

float UNotGDC_HealthComponent::ComputeDamageAmount(const FDamageInformation& DamageInformation) const
{
	if(!bLoadedHealthConfiguration)
	{
		LOG_NOTGDC(LogHealthComponent, Error, "Health Configuration has not been loaded");
		return 0.0f;
	}
	const TSoftClassPtr<UDamageType> SoftDamageType(DamageInformation.Type);
	const float* const MultiplierPtr = LoadedHealthConfigurationAsset->DamageMultipliers.Find(SoftDamageType);
	if(MultiplierPtr == nullptr)
	{
		return -DamageInformation.Amount;
	}
	return -(DamageInformation.Amount * (*MultiplierPtr));
}

void UNotGDC_HealthComponent::NotifyDamage(const FDamageInformation& DamageInformation)
{
	if(!bLoadedHealthConfiguration)
	{
		LOG_NOTGDC(LogHealthComponent, Error, "Health Configuration has not been loaded");
		return;
	}
	if(bDead)
	{
		return;
	}
	ApplyDamage(DamageInformation);
}

void UNotGDC_HealthComponent::ApplyDamage_Implementation(const FDamageInformation& DamageInformation)
{
	const float UnClampedHealth = Health + ComputeDamageAmount(DamageInformation);
	Health = FMath::Clamp(UnClampedHealth, -1.0f, MaxHealth);
	if(Health <= 0.0f)
	{
		OnDeath(DamageInformation);
	}
	OnDamageDelegate.Broadcast(this, DamageInformation);
}

void UNotGDC_HealthComponent::OnDeath(const FDamageInformation& DamageInformation)
{
	bDead = true;
	Health = 0.0f;
	BP_OnDeath(DamageInformation);
	OnDeathDelegate.Broadcast(this, DamageInformation.Causer);
}

void UNotGDC_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if(HealthConfigurationId.IsValid())
	{
		LoadHealthConfigId();
	}
}

void UNotGDC_HealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnloadHealthConfigId();
}

void UNotGDC_HealthComponent::LoadHealthConfigId()
{
	if(!HealthConfigurationId.IsValid())
	{
		LOG_NOTGDC(LogHealthComponent, Error, "INVALID HealthConfigurationId on UNotGDC_HealthComponent(%s)", *GetName());
		return;
	}
	if(bLoadedHealthConfiguration)
	{
		LOG_NOTGDC(LogHealthComponent, Warning, "HealthConfigurationId(%s) already loaded", *HealthConfigurationId.ToString());
		return;
	}
	LoadedHealthConfigHandle = UAssetManager::Get().LoadPrimaryAsset(HealthConfigurationId, { "DamageTypes" },
		FStreamableDelegate::CreateUObject(this, &ThisClass::OnHealthConfigLoaded_Internal));

	if(!LoadedHealthConfigHandle.IsValid())
	{
		OnHealthConfigLoaded_Internal();
	}
}

void UNotGDC_HealthComponent::UnloadHealthConfigId()
{
	LoadedHealthConfigurationAsset = nullptr;
	if(!LoadedHealthConfigHandle.IsValid() && !bLoadedHealthConfiguration)
	{
		bLoadedHealthConfiguration = false;
		return;
	}
	bLoadedHealthConfiguration = false;
	LoadedHealthConfigHandle.Reset();
	UAssetManager::Get().UnloadPrimaryAsset(HealthConfigurationId);
}

void UNotGDC_HealthComponent::OnHealthConfigLoaded_Internal()
{
	LoadedHealthConfigurationAsset = UAssetManager::Get().GetPrimaryAssetObject<UHealthConfigurationPrimaryDataAsset>(HealthConfigurationId);
	if(!IsValid(LoadedHealthConfigurationAsset))
	{
		LOG_NOTGDC(LogHealthComponent, Error, "Failed to get a valid LoadedHealthConfigurationAsset on UNotGDC_HealthComponent(%s)", *GetName());
		return;
	}
	bLoadedHealthConfiguration = true;

	MaxHealth = LoadedHealthConfigurationAsset->DefaultHealth;
	Health = MaxHealth;

	OnHealthConfigLoaded();
	OnHealthConfigLoadedDelegate.Broadcast(this);
}
