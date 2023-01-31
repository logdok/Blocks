// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BL_C_BuildingComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBL_C_BuilderComponent, All, All);

UBL_C_BuildingComponent::UBL_C_BuildingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBL_C_BuildingComponent::StartAction()
{
	UE_LOG(LogBL_C_BuilderComponent, Display, TEXT("--- Call Start Action"));
}

void UBL_C_BuildingComponent::EndAction()
{
	UE_LOG(LogBL_C_BuilderComponent, Display, TEXT("--- Call End Action"));
}

void UBL_C_BuildingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBL_C_BuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
