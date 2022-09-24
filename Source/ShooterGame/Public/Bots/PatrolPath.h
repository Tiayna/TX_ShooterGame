// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bots/SmartObject.h"
#include "PatrolPath.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API APatrolPath : public ASmartObject
{
	GENERATED_BODY()

public:

	APatrolPath();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SplinePath")
	class USplineComponent* Path;

	void GetSplinePoints();

	TArray<FVector> Locations;
	
};
