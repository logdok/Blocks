// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BL_C_BaseBlock.generated.h"

UCLASS()
class BLOCKS_API ABL_C_BaseBlock : public AActor
{
	GENERATED_BODY()

public:
	ABL_C_BaseBlock();

	void onEndBuilding();
	
	void OnStartDestroy();
	void OnResetDestroy();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Components)
	UStaticMeshComponent* BL_MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Emit)
	float FadeTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Emit)
	float FadePowerMult = 8.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Emit)
	float RiseTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Emit)
	float RisePowerMult = 5.0f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	bool M_StartFadeEmit;
	float M_FXEmitPower;

	bool M_StartRiseEmit;
	bool M_ResetRiseEmit;

	UPROPERTY()
	UMaterialInstanceDynamic* M_BlockDynMat;

	void ChangeEmitPower(float Power);
	void ResetEmitToZero();
};
