#include "Character/BL_C_Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BL_C_BuildingComponent.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBL_C_Character, All, All);

ABL_C_Character::ABL_C_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	BL_CameraComponent = CreateDefaultSubobject<UCameraComponent>("BL_CameraComponent");
	BL_CameraComponent->SetupAttachment(GetRootComponent());
	BL_CameraComponent->bUsePawnControlRotation = true;

	BL_FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BL_FirstPersonMesh");
	BL_FirstPersonMesh->SetupAttachment(BL_CameraComponent);
	BL_FirstPersonMesh->CastShadow = false;

	BL_FirstPersonGun = CreateDefaultSubobject<USkeletalMeshComponent>("BL_FirstPersonGun");
	BL_FirstPersonGun->SetupAttachment(GetRootComponent());
	BL_FirstPersonGun->CastShadow = false;
	
	BL_LightSphere = CreateDefaultSubobject<UStaticMeshComponent>("BL_LightSphere");
	BL_LightSphere->SetupAttachment(GetRootComponent());
	BL_LightSphere->CastShadow = false;

	BL_BuildingComponent = CreateDefaultSubobject<UBL_C_BuildingComponent>("BL_BuildingComponent");

	UE_LOG(LogBL_C_Character, Display, TEXT("---------------------- Sydch, End call Character constructor"));
}

void ABL_C_Character::StartAction()
{
	BL_BuildingComponent->StartAction();
}

void ABL_C_Character::EndAction()
{
	BL_BuildingComponent->EndAction();
}

void ABL_C_Character::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	check(BL_CameraComponent);
	check(BL_FirstPersonMesh);
	check(BL_FirstPersonGun);
	check(BL_LightSphere);

	BL_FirstPersonGun->AttachToComponent(BL_FirstPersonMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("GripPoint"));
	BL_LightSphere->AttachToComponent(BL_FirstPersonGun, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Muzzle"));
	
}

void ABL_C_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABL_C_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//PrimaryAction
		EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &ABL_C_Character::StartAction);
		EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Completed, this, &ABL_C_Character::EndAction);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABL_C_Character::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABL_C_Character::Look);
	}
}

void ABL_C_Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ABL_C_Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
