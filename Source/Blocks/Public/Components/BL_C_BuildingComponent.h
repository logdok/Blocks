// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blocks/BL_Variables.h"
#include "BL_C_BuildingComponent.generated.h"

class ABL_C_Character;
class ABL_C_BaseBlock;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLOCKS_API UBL_C_BuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBL_C_BuildingComponent();

	void StartAction();
	void EndAction();
	void ChangeMaterial(float Value);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Subclass)
	TSubclassOf<AActor> BigBlockClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Trace)
	float MaxTraceDistance = 3000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Trace)
	float WithoutHitDistance = 750.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Materials)
	TArray<FBlockMaterialPair> BlockMaterialPairs;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool M_isStartBuilding;
	bool M_isStartDestroy;
	bool M_isStartPreview;

	UPROPERTY()
	ABL_C_Character* M_Owner;

	UPROPERTY()
	ABL_C_BaseBlock* M_CurrentBlock;

	UPROPERTY()
	UMaterialInstanceDynamic* M_CurrentMat;

	EActionType M_CurrentAction;

	int32 M_CurrentMaterialIndex;

	int32 M_DeltaIndex;

	FVector M_BlockLoc;

	void DrawTrace(TArray<AActor*> IgnoredActors, FHitResult& HitResult, float Distance);
	bool CreateBlock(const FHitResult& HitResult);
	void CalculateStartEndLocation(float Distance, FVector& StartLoc, FVector& EndLoc);
	void SetBlockLocation(const FHitResult& HitResult);
	void CreateAndSetMaterial(UMaterialInterface* ParentMaterial);
};
