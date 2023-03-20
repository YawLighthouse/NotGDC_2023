// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NotGDC_HealthComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHealthComponent, Log, All);

struct FStreamableHandle;

class UHealthConfigurationPrimaryDataAsset;

/** Container for damage information, for easily passing around different systems. */
USTRUCT(BlueprintType)
struct NOTGDC_2023_API FDamageInformation
{
	GENERATED_BODY()
public:

	FDamageInformation()
	{ }

	/** The actor that caused the damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Causer = nullptr;
	
	/** The damage amount to apply */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount = 100.0f;

	/** The damage type to apply */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> Type = nullptr;
	
};

class UNotGDC_HealthComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthComponentDelegate, UNotGDC_HealthComponent*, HealthComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthComponentDeathDelegate, UNotGDC_HealthComponent*, HealthComponent, AActor*, DeathCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthComponentDamageDelegate, UNotGDC_HealthComponent*, HealthComponent, const FDamageInformation&, DamageInformation);

/**
 * 
 */
UCLASS(ClassGroup = "NotGDC", BlueprintType)
class NOTGDC_2023_API UNotGDC_HealthComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	
	UNotGDC_HealthComponent();

	UFUNCTION(BlueprintPure, Category = "Health Component")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "Health Component")
	float GetHealthRatio() const { return Health / MaxHealth; }
	
	UFUNCTION(BlueprintPure, Category = "Health Component")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category = "Health Component")
	FPrimaryAssetId GetHealthConfigId() const { return HealthConfigurationId; }
	
	UFUNCTION(BlueprintPure, Category = "Health Component")
	UHealthConfigurationPrimaryDataAsset* GetHealthConfigAsset() const { return LoadedHealthConfigurationAsset; }

	UFUNCTION(BlueprintPure, Category = "Health Component")
	float ComputeDamageAmount(const FDamageInformation& DamageInformation) const;
	
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void NotifyDamage(const FDamageInformation& DamageInformation);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Health Component")
	void ApplyDamage(const FDamageInformation& DamageInformation);

	UPROPERTY(BlueprintAssignable, Category = "Health Component")
	FOnHealthComponentDelegate OnHealthConfigLoadedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Health Component")
	FOnHealthComponentDamageDelegate OnDamageDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Health Component")
	FOnHealthComponentDeathDelegate OnDeathDelegate;
	
protected:

	virtual void ApplyDamage_Implementation(const FDamageInformation& DamageInformation);

	// AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~AActor interface

	virtual void OnDeath(const FDamageInformation& DamageInformation);
	UFUNCTION(BlueprintImplementableEvent, Category = "Health Component", meta = (DisplayName = "On Death"))
	void BP_OnDeath(const FDamageInformation& DamageInformation);
	
	virtual void LoadHealthConfigId();
	virtual void OnHealthConfigLoaded() { }
	UFUNCTION(BlueprintImplementableEvent, Category = "Health Component", meta = (DisplayName = "On Health Config Loaded"))
	void BP_OnHealthConfigLoaded();
	virtual void UnloadHealthConfigId();
	
	UPROPERTY(EditAnywhere, Category = "Health Component", meta = (AllowedTypes = "HealthConfigurationPrimaryDataAsset"))
	FPrimaryAssetId HealthConfigurationId;

	UPROPERTY(VisibleInstanceOnly, Transient, Category = "Health Component")
	TObjectPtr<UHealthConfigurationPrimaryDataAsset> LoadedHealthConfigurationAsset;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Transient, Category = "Health Component")
	float MaxHealth;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Transient, Category = "Health Component")
	float Health;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Transient, Category = "Health Component")
	uint8 bDead : 1;

	TSharedPtr<FStreamableHandle> LoadedHealthConfigHandle;

private:

	void OnHealthConfigLoaded_Internal();
	
	UPROPERTY(Transient)
	uint8 bLoadedHealthConfiguration : 1;
	
};
