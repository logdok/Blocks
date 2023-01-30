#include "Character/BL_C_Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBL_Character, All, All);

ABL_C_Character::ABL_C_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	BL_CameraComponent = CreateDefaultSubobject<UCameraComponent>("BL_CameraComponent");
	BL_CameraComponent->SetupAttachment(GetRootComponent());
	BL_CameraComponent->bUsePawnControlRotation = true;

	BL_FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BL_FirstPersonMesh");
	BL_FirstPersonMesh->SetupAttachment(BL_CameraComponent);
	BL_FirstPersonMesh->SetOnlyOwnerSee(true);
	BL_FirstPersonMesh->bCastDynamicShadow = false;
	BL_FirstPersonMesh->CastShadow = false;

	UE_LOG(LogBL_Character, Display, TEXT("---------------------- Sydch, End call Character constructor"));
}

void ABL_C_Character::BeginPlay()
{
	Super::BeginPlay();

	check(BL_CameraComponent);
	check(BL_FirstPersonMesh);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	else
	{
		int a = 0;
	}
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
