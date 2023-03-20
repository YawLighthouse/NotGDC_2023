// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NotGDC_Character.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogNotGDCCharacter, Log, All);

class UInputAction;
class UInputMappingContext;

class USpringArmComponent;
class UCameraComponent;
class UNotGDC_HealthComponent;

class UCharacterPrimaryDataAsset;
class UCharacterAbilitiesPrimaryDataAsset;

struct FStreamableHandle;

class ANotGDC_Character;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharacterAbilityChangedDelegate, ANotGDC_Character*, Character, UCharacterAbilitiesPrimaryDataAsset*, Ability);

/**
 * 
 */
UCLASS(ClassGroup = "NotGDC")
class NOTGDC_2023_API ANotGDC_Character : public ACharacter
{
	GENERATED_BODY()
public:
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UNotGDC_HealthComponent* GetHealthComponent() const { return HealthComponent; }
	
	ANotGDC_Character();

	UFUNCTION(BlueprintPure, Category = "NotGDC Character")
	FPrimaryAssetId GetCharacterId() const { return CharacterId; }

	UFUNCTION(BlueprintPure, Category = "NotGDC Character|Abilities")
	UCharacterAbilitiesPrimaryDataAsset* GetAbilityAsset() const { return AbilityAsset; }

	UFUNCTION(BlueprintCallable, Category = "NotGDC Character|Abilities")
	void SetAbilityAsset(UCharacterAbilitiesPrimaryDataAsset* NewAbility);

	UPROPERTY(BlueprintAssignable, Category = "NotGDC Character|Abilities")
	FCharacterAbilityChangedDelegate OnAbilityChanged;
	
protected:

	void StartJumping();

	void UseAbility();
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~AActor interface

	virtual void LoadCharacterId();
	virtual void OnCharacterDataAssetLoaded() { }
	UFUNCTION(BlueprintImplementableEvent, Category = "NotGDC Character", meta = (DisplayName = "On Character Data Asset Loaded"))
	void BP_OnCharacterDataAssetLoaded();
	virtual void UnloadCharacterId();
	
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// ~APawn interface
	
	UPROPERTY(EditAnywhere, Category = "NotGDC Character", meta = (AllowedTypes = "CharacterPrimaryDataAsset"))
	FPrimaryAssetId CharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NotGDC Character")
	TArray<FName> LoadBundles = { "UI", "Abilities" };
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "NotGDC Character")
	TObjectPtr<UCharacterPrimaryDataAsset> LoadedCharacterDataAsset;
	
	TSharedPtr<FStreamableHandle> LoadedCharacterDataAssetHandle;
	
	UPROPERTY(VisibleInstanceOnly, Category = "NotGDC Character|Abilities")
	TObjectPtr<UCharacterAbilitiesPrimaryDataAsset> JumpAbilityAsset;
	
	UPROPERTY(VisibleInstanceOnly, Category = "NotGDC Character|Abilities")
	TObjectPtr<UCharacterAbilitiesPrimaryDataAsset> AbilityAsset;

	/** Health management component for this character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UNotGDC_HealthComponent> HealthComponent;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	/** Use Ability Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> UseAbilityAction;
	
private:

	void OnCharacterIdLoaded_Internal();

	UPROPERTY(Transient)
	uint8 bLoadedCharacterDataAsset : 1;
	
};

