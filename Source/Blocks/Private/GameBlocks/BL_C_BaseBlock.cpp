// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBlocks/BL_C_BaseBlock.h"

ABL_C_BaseBlock::ABL_C_BaseBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	BL_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BL_MeshComponent");
	SetRootComponent(BL_MeshComponent);
}

void ABL_C_BaseBlock::BeginPlay()
{
	Super::BeginPlay();
}

void ABL_C_BaseBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
