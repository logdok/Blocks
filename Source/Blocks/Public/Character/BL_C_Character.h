#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BL_C_Character.generated.h"

class UCameraComponent;
UCLASS()
class BLOCKS_API ABL_C_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ABL_C_Character();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UCameraComponent* BL_CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* BL_FirstPersonMesh;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
