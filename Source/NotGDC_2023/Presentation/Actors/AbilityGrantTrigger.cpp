// Copyright 2023 Nicholas Helish. All rights reserved.


#include "AbilityGrantTrigger.h"

#include "NotGDC_Character.h"
#include "NotGDC_Logging.h"
#include "Components/BoxComponent.h"
#include "Engine/AssetManager.h"
#include "PrimaryDataAssetTypes/CharacterAbilitiesPrimaryDataAsset.h"

DEFINE_LOG_CATEGORY(LogAbilityGrantTrigger);

AAbilityGrantTrigger::AAbilityGrantTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	BoxTriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTriggerComponent"));
	
	BoxTriggerComponent->SetBoxExtent(FVector(100.0f));
	BoxTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxTriggerOverlap);
	BoxTriggerComponent->SetupAttachment(RootComponent);
}

void AAbilityGrantTrigger::BeginPlay()
{
	Super::BeginPlay();

	// Valid check it without an error log here because its begin play, so this is expected behavior where its may be ok to have it null.
	if(AbilityId.IsValid())
	{
		LoadAbilityId();
	}
}

void AAbilityGrantTrigger::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	UnloadAbilityId();	
}

void AAbilityGrantTrigger::OnBoxTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANotGDC_Character* const OverlappedCharacter = Cast<ANotGDC_Character>(OtherActor);
	if(!IsValid(OverlappedCharacter))
	{
		return;
	}
	GrantAbilityToCharacter(OverlappedCharacter);
}

void AAbilityGrantTrigger::LoadAbilityId()
{
	if(!AbilityId.IsValid())
	{
		LOG_NOTGDC(LogAbilityGrantTrigger, Error, "INVALID AbilityId on AAbilityGrantTrigger(%s)", *GetName());
		return;
	}
	if(IsAbilityLoaded())
	{
		LOG_NOTGDC(LogAbilityGrantTrigger, Warning, "AbilityId(%s) already loaded", *AbilityId.ToString());
		return;
	}
	LoadedAbilityHandle = UAssetManager::Get().LoadPrimaryAsset(AbilityId, LoadBundles,
		FStreamableDelegate::CreateUObject(this, &ThisClass::OnAbilityLoaded_Internal));
	
	if(LoadedAbilityHandle.IsValid())
	{
		OnAbilityLoadStart();
	}
	else
	{
		OnAbilityLoaded_Internal();
	}
}

void AAbilityGrantTrigger::OnAbilityLoaded_Internal()
{
	LoadedAbilityAsset = UAssetManager::Get().GetPrimaryAssetObject<UCharacterAbilitiesPrimaryDataAsset>(AbilityId);
	if(!IsValid(LoadedAbilityAsset))
	{
		LOG_NOTGDC(LogAbilityGrantTrigger, Error, "Failed to get a valid LoadedAbilityAsset on AAbilityGrantTrigger(%s)", *GetName());
		return;
	}
	
	bLoadedAbility = true;
	OnAbilityLoaded();
	BP_OnAbilityLoaded();
}

void AAbilityGrantTrigger::UnloadAbilityId()
{
	LoadedAbilityAsset = nullptr;
	if(!LoadedAbilityHandle.IsValid() && !IsAbilityLoaded())
	{
		bLoadedAbility = false;
		return;
	}
	bLoadedAbility = false;
	LoadedAbilityHandle.Reset();
	UAssetManager::Get().UnloadPrimaryAsset(AbilityId);
}

