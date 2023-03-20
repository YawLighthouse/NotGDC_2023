// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityGrantTrigger.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAbilityGrantTrigger, Log, All);

struct FStreamableHandle;

class UBoxComponent;
class ANotGDC_Character;

UCLASS(ClassGroup = "NotGDC", Blueprintable, BlueprintType)
class NOTGDC_2023_API AAbilityGrantTrigger : public AActor
{
	GENERATED_BODY()
public:
	
	AAbilityGrantTrigger();

	UFUNCTION(BlueprintPure, Category = "Abilities")
	bool IsAbilityLoaded() const { return bLoadedAbility; }

	UFUNCTION(BlueprintPure, Category = "Abilities")
	FPrimaryAssetId GetAbilityId() const { return AbilityId; }
	
	UFUNCTION(BlueprintPure, Category = "Abilities")
	UCharacterAbilitiesPrimaryDataAsset* GetAbilityAsset() const { return LoadedAbilityAsset; }

	UFUNCTION(BlueprintNativeEvent, Category = "Abilities")
	void GrantAbilityToCharacter(ANotGDC_Character* Character);
	
protected:

	// AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~AActor interface

	UFUNCTION()
	void OnBoxTriggerOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void LoadAbilityId();
	UFUNCTION(BlueprintNativeEvent, Category = "Abilities")
	void OnAbilityLoadStart();
	virtual void OnAbilityLoadStart_Implementation() { }
	virtual void OnAbilityLoaded() { }
	UFUNCTION(BlueprintImplementableEvent, Category = "Abilities", meta = (DisplayName = "On Ability Loaded"))
	void BP_OnAbilityLoaded();
	virtual void UnloadAbilityId();
	
	virtual void GrantAbilityToCharacter_Implementation(ANotGDC_Character* Character) { }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> BoxTriggerComponent;
	
	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (AllowedTypes = "CharacterAbilitiesPrimaryDataAsset"))
	FPrimaryAssetId AbilityId;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<FName> LoadBundles = { "UI" };
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Abilities")
	TObjectPtr<UCharacterAbilitiesPrimaryDataAsset> LoadedAbilityAsset;
	
	TSharedPtr<FStreamableHandle> LoadedAbilityHandle;
	
private:

	void OnAbilityLoaded_Internal();
	
	UPROPERTY(Transient)
	uint8 bLoadedAbility : 1;
	
};
