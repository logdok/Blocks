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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Components)
	UStaticMeshComponent* BL_MeshComponent;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
