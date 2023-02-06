// Fill out your copyright notice in the Description page of Project Settings.


#include "Flash/BL_C_FlashImage.h"

ABL_C_FlashImage::ABL_C_FlashImage()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);
}

void ABL_C_FlashImage::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABL_C_FlashImage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

