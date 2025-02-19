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
}

void UBL_C_BuildingComponent::EndAction()
{
	if (M_CurrentAction == EActionType::Building)
	{
		M_isStartBuilding = false;
		M_isStartPreview = false;
		CreateAndSetMaterial(BlockMaterialPairs[M_CurrentMaterialIndex].Base);
		if(IsValid(M_CurrentMat) && IsValid(M_CurrentBlock))
		{
			M_CurrentMat->SetVectorParameterValue(FName("Emit Color"), BlockMaterialPairs[M_CurrentMaterialIndex].EmitColor);
			M_CurrentBlock->onEndBuilding();
		}
	}
	if (M_CurrentAction == EActionType::Destroy)
	{
		M_isStartDestroy = false;
		// notify previous actor about stop destroy 
		Cast<ABL_C_BaseBlock>(M_HitActor)->OnResetDestroy();
	}
}

void UBL_C_BuildingComponent::ChangeMaterial(float Value)
{
	M_CurrentMaterialIndex += FMath::Clamp(Value, -1.0f, 1.0f);

	if (BlockMaterialPairs.Num() == M_CurrentMaterialIndex)
	{
		M_CurrentMaterialIndex = 0;
	}
	else if (-1 == M_CurrentMaterialIndex)
	{
		M_CurrentMaterialIndex = BlockMaterialPairs.Num() - 1;
	}
	if(M_isStartPreview)
	{
		CreateAndSetMaterial(BlockMaterialPairs[M_CurrentMaterialIndex].Preview);
	}

	UE_LOG(LogBL_C_BuilderComponent, Display, TEXT("-------- Current Material Index: %i"), M_CurrentMaterialIndex);
}

void UBL_C_BuildingComponent::ChangeBlock()
{
	++M_CurrentBlockIndex;

	if (BlockClasses.Num() == M_CurrentBlockIndex)
	{
		M_CurrentBlockIndex = 0;
	}
	else if (-1 == M_CurrentBlockIndex)
	{
		M_CurrentBlockIndex = BlockClasses.Num() - 1;
	}
	if(M_isStartPreview)
	{
		M_CurrentBlock->Destroy();
		M_isStartPreview = false;
		M_isStartBuilding = true; // need check
	}
}

void UBL_C_BuildingComponent::SwitchAction()
{
	if(EActionType::Destroy == M_CurrentAction || EActionType::None == M_CurrentAction)
	{
		M_CurrentAction = EActionType::Building;
		M_LightSphereMat->SetVectorParameterValue(FName("Color"), FLinearColor::Green);
	}
	else
	{
		M_CurrentAction = EActionType::Destroy;
		M_LightSphereMat->SetVectorParameterValue(FName("Color"), FLinearColor::Red);
	}
	UE_LOG(LogBL_C_BuilderComponent, Display, TEXT("Switch action..."));
}

void UBL_C_BuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	M_Owner = Cast<ABL_C_Character>(GetOwner());
	M_CurrentAction = EActionType::Building;
	M_CurrentMaterialIndex = 0;
	if(IsValid(M_Owner) &&
		IsValid(M_Owner->BL_LightSphere) &&
		IsValid(M_Owner->BL_LightSphere->GetMaterial(0)) &&
		IsValid(GetWorld()))
	{
		M_LightSphereMat = UMaterialInstanceDynamic::Create(M_Owner->BL_LightSphere->GetMaterial(0), GetWorld());
		if(M_LightSphereMat)
		{
			M_Owner->BL_LightSphere->SetMaterial(0, M_LightSphereMat);
		} 
	}
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
			SetBlockLocation(HitResult);
		}
	}
	else if (M_isStartDestroy)
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(M_Owner);

		FHitResult HitResult;
		DrawTrace(IgnoredActors, HitResult, MaxTraceDistance);
		if(HitResult.bBlockingHit &&
			IsValid(HitResult.GetActor()))
		{
			if(HitResult.GetActor()->IsA<ABL_C_BaseBlock>())
			{
				Cast<ABL_C_BaseBlock>(HitResult.GetActor())->OnStartDestroy();
			}

			if(IsValid(M_HitActor) && M_HitActor->GetName() != HitResult.GetActor()->GetName())
			{
				// notify previous actor about stop destroy 
				Cast<ABL_C_BaseBlock>(M_HitActor)->OnResetDestroy();
			}
			M_HitActor = HitResult.GetActor();
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
	                                      EDrawDebugTrace::None,
	                                      HitResult,
	                                      true,
	                                      FLinearColor::Red,
	                                      FLinearColor::Green,
	                                      0.5f);
}

bool UBL_C_BuildingComponent::CreateBlock(const FHitResult& HitResult)
{
	if (M_isStartPreview) return true;
	if (!IsValid(BlockClasses[M_CurrentBlockIndex])) return false;

	FTransform Transform;
	Transform.SetLocation(HitResult.Location);
	M_CurrentBlock = GetWorld()->SpawnActor<ABL_C_BaseBlock>(BlockClasses[M_CurrentBlockIndex], Transform);
	if(IsValid(M_CurrentBlock))
	{
		FVector Origin(ForceInitToZero), Extend(ForceInitToZero);
		
		M_CurrentBlock->GetActorBounds(false, Origin, Extend);
		M_CurrentBlockExtend = Extend.X;
	}
	
	CreateAndSetMaterial(BlockMaterialPairs[M_CurrentMaterialIndex].Preview);
	return M_isStartPreview = IsValid(M_CurrentBlock);
}

void UBL_C_BuildingComponent::CalculateStartEndLocation(float Distance, FVector& StartLoc, FVector& EndLoc)
{
	StartLoc = M_Owner->BL_LightSphere->GetComponentLocation();
	EndLoc = StartLoc + M_Owner->FindComponentByClass<UCameraComponent>()->GetForwardVector() * Distance;
}

void UBL_C_BuildingComponent::SetBlockLocation(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		M_BlockLoc = HitResult.Location.GridSnap(M_CurrentBlockExtend) + HitResult.Normal * M_CurrentBlockExtend;
	}
	else
	{
		FVector StartLoc(ForceInitToZero), EndLoc(ForceInitToZero);
		CalculateStartEndLocation(WithoutHitDistance, StartLoc, EndLoc);
		M_BlockLoc = EndLoc.GridSnap(M_CurrentBlockExtend);
	}

	const TArray<AActor*> IgnoredBlocks = {M_Owner, M_CurrentBlock};
	TArray<FHitResult> BoxHits;

	UKismetSystemLibrary::BoxTraceMulti(GetWorld(),
	                                    M_BlockLoc,
	                                    M_BlockLoc,
	                                    FVector(M_CurrentBlockExtend),
	                                    FRotator::ZeroRotator,
	                                    TraceTypeQuery1,
	                                    false,
	                                    IgnoredBlocks,
	                                    EDrawDebugTrace::None,
	                                    BoxHits,
	                                    true
	);

	for (const auto& OneHit : BoxHits)
	{
		M_BlockLoc += OneHit.Normal;
		//UE_LOG(LogBL_C_BuilderComponent, Display, TEXT("Hit: %s"), *OneHit.Normal.ToString());
	}

	if(IsValid(M_CurrentBlock))
	{
		M_CurrentBlock->SetActorLocation(M_BlockLoc);
	}
}

void UBL_C_BuildingComponent::CreateAndSetMaterial(UMaterialInterface* ParentMaterial)
{
	if (IsValid(M_CurrentBlock) && IsValid(GetWorld()) && IsValid(ParentMaterial))
	{
		M_CurrentMat = UMaterialInstanceDynamic::Create(ParentMaterial, GetWorld());
		M_CurrentBlock->BL_MeshComponent->SetMaterial(0, M_CurrentMat);
		M_DeltaIndex = M_CurrentMaterialIndex;
	}
}
