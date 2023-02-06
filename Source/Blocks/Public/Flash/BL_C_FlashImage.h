// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BL_C_FlashImage.generated.h"

UCLASS()
class BLOCKS_API ABL_C_FlashImage : public AActor
{
	GENERATED_BODY()
	
public:	
	ABL_C_FlashImage();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Components)
	UStaticMeshComponent* MeshComponent;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
