#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BL_C_Character.generated.h"

class UInputComponent;
class UCameraComponent;
class UBL_C_BuildingComponent;

UCLASS()
class BLOCKS_API ABL_C_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ABL_C_Character();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=Mesh)
	UStaticMeshComponent* BL_LightSphere;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera, meta = (AllowPrivateAccess="true"))
	UCameraComponent* BL_CameraComponent;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* BL_FirstPersonMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* BL_FirstPersonGun;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=Components)
	UBL_C_BuildingComponent* BL_BuildingComponent;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta = (AllowPrivateAccess="true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* PrimaryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ChangeMaterialAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SwitchMaterialAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ChangeBlockAction;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USkeletalMeshComponent* GetBL_FirstPersonMesh() const { return BL_FirstPersonMesh; }

	UCameraComponent* GetBL_CameraComponent() const { return BL_CameraComponent; }

private:
	void StartAction();
	void EndAction();
	void SwitchAction();
	void ChangeAction();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void ChangeMaterial(const FInputActionValue& Value);
};
