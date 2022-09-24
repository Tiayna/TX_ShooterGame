// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Bots/PatrolPath.h"

#include <filesystem>

#include "Components/SplineComponent.h"

APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick=false;

	Path=CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	Path->SetupAttachment(RootComponent);
	Path->bDrawDebug=true;
	
	Path->AddSplinePoint(FVector(100,100,0),ESplineCoordinateSpace::Local);
	Path->AddSplinePoint(FVector(-100,100,0),ESplineCoordinateSpace::Local);
	Path->AddSplinePoint(FVector(-100,-100,0),ESplineCoordinateSpace::Local);
	Path->AddSplinePoint(FVector(100,-100,0),ESplineCoordinateSpace::Local);
}

void APatrolPath::BeginPlay()
{
	Super::BeginPlay();
	GetSplinePoints();   //For a loop
}

void APatrolPath::GetSplinePoints()
{
	for(int i=0;i<=Path->GetNumberOfSplinePoints();++i)
	{
		Locations.Add(Path->GetLocationAtSplinePoint(i,ESplineCoordinateSpace::World));    //该PatrolPath实例用于向blackboard中设置巡逻路线
	}
}
