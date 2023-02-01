// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBlocks/BL_C_BaseBlock.h"

ABL_C_BaseBlock::ABL_C_BaseBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	BL_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BL_MeshComponent");
	SetRootComponent(BL_MeshComponent);
}

void ABL_C_BaseBlock::onEndBuilding()
{
	M_StartFadeEmit = true;
	if(IsValid(BL_MeshComponent) && IsValid(BL_MeshComponent->GetMaterial(0)))
	{
		M_BlockDynMat = Cast<UMaterialInstanceDynamic>(BL_MeshComponent->GetMaterial(0));
	}
}

void ABL_C_BaseBlock::BeginPlay()
{
	Super::BeginPlay();

	M_FXEmitPower = FadeTime;
}

void ABL_C_BaseBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(M_StartFadeEmit)
	{
		if(M_FXEmitPower > 0)
		{
			M_FXEmitPower -= DeltaTime;
			ChangeEmitPower(M_FXEmitPower * FadePowerMult);
		}
		else
		{
			ResetEmitToZero();
		}
	}
}

void ABL_C_BaseBlock::ChangeEmitPower(float Power)
{
	if(!IsValid(M_BlockDynMat)) return;

	M_BlockDynMat->SetScalarParameterValue(FName("Power"), Power * Power);
}

void ABL_C_BaseBlock::ResetEmitToZero()
{
	ChangeEmitPower(0.0f);
	M_FXEmitPower = 0.0f;
	M_StartFadeEmit = false;
}
