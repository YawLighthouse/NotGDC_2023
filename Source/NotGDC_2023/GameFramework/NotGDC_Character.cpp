// Copyright 2023 Nicholas Helish. All rights reserved.

#include "NotGDC_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NotGDC_HealthComponent.h"
#include "NotGDC_Logging.h"
#include "ConfigurationTypes/AbilityConfiguration.h"
#include "Engine/AssetManager.h"
#include "PrimaryDataAssetTypes/CharacterPrimaryDataAsset.h"
#include "PrimaryDataAssetTypes/CharacterAbilitiesPrimaryDataAsset.h"

DEFINE_LOG_CATEGORY(LogNotGDCCharacter);

ANotGDC_Character::ANotGDC_Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	HealthComponent = CreateDefaultSubobject<UNotGDC_HealthComponent>(TEXT("HealthComponent"));
}

void ANotGDC_Character::SetAbilityAsset(UCharacterAbilitiesPrimaryDataAsset* NewAbility)
{
	AbilityAsset = NewAbility;
	OnAbilityChanged.Broadcast(this, AbilityAsset);
}

void ANotGDC_Character::StartJumping()
{
	if(!IsValid(JumpAbilityAsset))
	{
		LOG_SCREEN_NOTGDC(LogNotGDCCharacter, 5.0f, Error, "NULL JumpAbility on ANotGDC_Character(%s)", *GetName());
		return;
	}
	UAbilityConfiguration* const JumpAbility = JumpAbilityAsset->GetFirstAbilityConfig();
	if(!IsValid(JumpAbility))
	{
		LOG_SCREEN_NOTGDC(LogNotGDCCharacter, 5.0f, Error, "NULL JumpAbility from JumpAbilityAsset(%s) at index 0", *GetNameSafe(JumpAbilityAsset));
		return;
	}
	JumpAbility->UseAbility(this);
}

void ANotGDC_Character::UseAbility()
{
	if(!IsValid(AbilityAsset))
	{
		LOG_NOTGDC(LogNotGDCCharacter, Log, "NULL AbilityAsset on ANotGDC_Character(%s)", *GetName());
		return;
	}
	UAbilityConfiguration* const Ability = AbilityAsset->GetFirstAbilityConfig();
	if(!IsValid(Ability))
	{
		LOG_SCREEN_NOTGDC(LogNotGDCCharacter, 5.0f, Error, "NULL Ability from AbilityAsset(%s) at index 0", *GetNameSafe(AbilityAsset));
		return;
	}
	Ability->UseAbility(this);
}

void ANotGDC_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* const PlayerController = GetController<APlayerController>();
	if(!IsValid(PlayerController))
	{
		LOG_SCREEN_NOTGDC(LogNotGDCCharacter, 5.0f, Error, "NULL PlayerController on ANotGDC_Character(%s)", *GetName());
		return;
	}
	const ULocalPlayer* const LocalPlayer = PlayerController->GetLocalPlayer();
	if(!IsValid(LocalPlayer))
	{
		LOG_SCREEN_NOTGDC(LogNotGDCCharacter, 5.0f, Error, "NULL LocalPlayer on PlayerController(%s)", *GetNameSafe(PlayerController));
		return;
	}
	UEnhancedInputLocalPlayerSubsystem* const Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!IsValid(Subsystem))
	{
		LOG_SCREEN_NOTGDC(LogNotGDCCharacter, 5.0f, Error, "NULL UEnhancedInputLocalPlayerSubsystem on LocalPlayer(%s)", *GetNameSafe(LocalPlayer));
		return;
	}
	//Add Input Mapping Context
	Subsystem->AddMappingContext(DefaultMappingContext, 0);
	
	// Set up action bindings
	UEnhancedInputComponent* const EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (!IsValid(EnhancedInputComponent))
	{
		LOG_SCREEN_NOTGDC(LogNotGDCCharacter, 5.0f, Error, "PlayerInputComponent(%s) is not of type UEnhancedInputComponent", *GetNameSafe(PlayerInputComponent));
		return;
	}
	
	// Jumping
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ANotGDC_Character::StartJumping);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	// Moving
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANotGDC_Character::Move);

	// Looking
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANotGDC_Character::Look);

	// Using Ability
	EnhancedInputComponent->BindAction(UseAbilityAction, ETriggerEvent::Triggered, this, &ANotGDC_Character::UseAbility);
}

void ANotGDC_Character::Move(const FInputActionValue& Value)
{
	if (!IsValid(Controller))
	{
		// Exit to account for the controller might not be valid yet or no longer possessed
		return;
	}
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ANotGDC_Character::Look(const FInputActionValue& Value)
{
	if (!IsValid(Controller))
	{
		// Exit to account for the controller might not be valid yet or no longer possessed
		return;
	}
	
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	// add yaw and pitch input to controller
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ANotGDC_Character::LoadCharacterId()
{
	if(!CharacterId.IsValid())
	{
		LOG_NOTGDC(LogNotGDCCharacter, Error, "INVALID CharacterId on ANotGDC_Character(%s)", *GetName());
		return;
	}
	LoadedCharacterDataAssetHandle = UAssetManager::Get().LoadPrimaryAsset(CharacterId, LoadBundles,
		FStreamableDelegate::CreateUObject(this, &ThisClass::OnCharacterIdLoaded_Internal));
	if(!LoadedCharacterDataAssetHandle.IsValid())
	{
		OnCharacterIdLoaded_Internal();	
	}
}

void ANotGDC_Character::UnloadCharacterId()
{
	LoadedCharacterDataAsset = nullptr;
	if(!LoadedCharacterDataAssetHandle.IsValid() && !bLoadedCharacterDataAsset)
	{
		bLoadedCharacterDataAsset = false;
		return;
	}
	bLoadedCharacterDataAsset = false;
	LoadedCharacterDataAssetHandle.Reset();
	UAssetManager::Get().UnloadPrimaryAsset(CharacterId);
}

void ANotGDC_Character::OnCharacterIdLoaded_Internal()
{
	LoadedCharacterDataAsset = UAssetManager::Get().GetPrimaryAssetObject<UCharacterPrimaryDataAsset>(CharacterId);
	if(!IsValid(LoadedCharacterDataAsset))
	{
		LOG_NOTGDC(LogNotGDCCharacter, Error, "Failed to get a valid LoadedCharacterDataAsset on ANotGDC_Character(%s)", *GetName());
		return;
	}
	
	JumpAbilityAsset = LoadedCharacterDataAsset->JumpAbilityPath.Get();
	SetAbilityAsset(LoadedCharacterDataAsset->GetFirstDefaultAbility().Get());
	
	bLoadedCharacterDataAsset = true;
	OnCharacterDataAssetLoaded();
	BP_OnCharacterDataAssetLoaded();
}

void ANotGDC_Character::BeginPlay()
{
	Super::BeginPlay();

	if(CharacterId.IsValid())
	{
		LoadCharacterId();
	}
}

void ANotGDC_Character::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UnloadCharacterId();
}




