
#include "Character/BL_C_Character.h"

#include "Camera/CameraComponent.h"

ABL_C_Character::ABL_C_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	BL_CameraComponent = CreateDefaultSubobject<UCameraComponent>("BL_CameraComponent");
	BL_CameraComponent->SetupAttachment(GetRootComponent());

	BL_FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BL_FirstPersonMesh");
	BL_FirstPersonMesh->SetupAttachment(BL_CameraComponent);
}

void ABL_C_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ABL_C_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABL_C_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
