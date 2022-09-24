// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmartObject.generated.h"

UCLASS()
class SHOOTERGAME_API ASmartObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmartObject();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components")
	class UBillboardComponent* Billboard;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components")
	class UArrowComponent* FacingDirection;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AI")
	class UBehaviorTree* SubTree;

	//FORCEINLINE UBehaviorTree* GetSubTree();
	
};
