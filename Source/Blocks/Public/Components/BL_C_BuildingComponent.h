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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Subclass)
	TSubclassOf<AActor> BigBlockClass;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	ABL_C_Character* M_Owner;
	
	UPROPERTY()
	ABL_C_BaseBlock* M_CurrentBlock;
		
	EActionType M_CurrentAction;
	bool M_isStartAction;

	void DrawTrace(FHitResult& HitResult);
};
