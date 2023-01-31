// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BL_C_BuildingComponent.h"

#include "Camera/CameraComponent.h"
#include "Character/BL_C_Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameBlocks/BL_C_BaseBlock.h"


DEFINE_LOG_CATEGORY_STATIC(LogBL_C_BuilderComponent, All, All);

UBL_C_BuildingComponent::UBL_C_BuildingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBL_C_BuildingComponent::StartAction()
{
	if (!IsValid(M_Owner)) return;

	if (M_CurrentAction == EActionType::Building)
	{
		M_isStartBuilding = true;
	}
	if (M_CurrentAction == EActionType::Destroy)
	{
		M_isStartDestroy = true;
	}
	/*
	GetWorld()->LineTraceSingleByChannel(HitResult,
	                                     StartLoc,
	                                     EndLoc,
	                                     ECC_Visibility,
	                                     FCollisionQueryParams::DefaultQueryParam,
	                                     FCollisionResponseParams::DefaultResponseParam);
	                                     */

	UE_LOG(LogBL_C_BuilderComponent, Display, TEXT("--- Call Start Action"));
}

void UBL_C_BuildingComponent::EndAction()
{
	if (M_CurrentAction == EActionType::Building)
	{
		M_isStartBuilding = false;
		M_isStartPreview = false;
	}
	if (M_CurrentAction == EActionType::Destroy)
	{
		M_isStartDestroy = true;
	}
	UE_LOG(LogBL_C_BuilderComponent, Display, TEXT("--- Call End Action"));
}

void UBL_C_BuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	M_Owner = Cast<ABL_C_Character>(GetOwner());
	M_CurrentAction = EActionType::Building;
}

void UBL_C_BuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (M_isStartBuilding)
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(M_Owner);
		IgnoredActors.AddUnique(M_CurrentBlock);

		FHitResult HitResult;
		DrawTrace(IgnoredActors, HitResult, MaxTraceDistance);

		if (CreateBlock(HitResult))
		{
			if (HitResult.bBlockingHit)
			{
				M_CurrentBlock->SetActorLocation(HitResult.Location);
			}
			else
			{
				FVector StartLoc(ForceInitToZero), EndLoc(ForceInitToZero);
				CalculateStartEndLocation(WithoutHitDistance, StartLoc, EndLoc);
				M_CurrentBlock->SetActorLocation(EndLoc);
			}
		}
	}
}

void UBL_C_BuildingComponent::DrawTrace(TArray<AActor*> IgnoredActors, FHitResult& HitResult, float Distance)
{
	FVector StartLoc(ForceInitToZero), EndLoc(ForceInitToZero);
	CalculateStartEndLocation(Distance, StartLoc, EndLoc);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
	                                      StartLoc,
	                                      EndLoc,
	                                      TraceTypeQuery1,
	                                      false,
	                                      IgnoredActors,
	                                      EDrawDebugTrace::ForOneFrame,
	                                      HitResult,
	                                      true,
	                                      FLinearColor::Red,
	                                      FLinearColor::Green,
	                                      0.5f);
}

bool UBL_C_BuildingComponent::CreateBlock(const FHitResult& HitResult)
{
	if (M_isStartPreview) return true;
	if (!IsValid(BigBlockClass)) return false;

	FTransform Transform;
	Transform.SetLocation(HitResult.Location);
	M_CurrentBlock = GetWorld()->SpawnActor<ABL_C_BaseBlock>(BigBlockClass, Transform);
	return M_isStartPreview = IsValid(M_CurrentBlock);
}

void UBL_C_BuildingComponent::CalculateStartEndLocation(float Distance, FVector& StartLoc, FVector& EndLoc)
{
	StartLoc = M_Owner->BL_LightSphere->GetComponentLocation();
	EndLoc = StartLoc + M_Owner->FindComponentByClass<UCameraComponent>()->GetForwardVector() * Distance;
}
