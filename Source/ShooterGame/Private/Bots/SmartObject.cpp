// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Bots/SmartObject.h"
#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"

// Sets default values
ASmartObject::ASmartObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Billboard=CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());

	FacingDirection=CreateDefaultSubobject<UArrowComponent>(TEXT("Facing Direction"));
	FacingDirection->SetupAttachment(Billboard);
	
}

// Called when the game starts or when spawned
void ASmartObject::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
// void ASmartObject::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

