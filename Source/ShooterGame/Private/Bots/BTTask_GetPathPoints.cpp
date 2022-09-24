// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Bots/BTTask_GetPathPoints.h"

#include "AIController.h"
#include "Components/SplineComponent.h"
#include "PatrolPath.h"
#include "ShooterBot.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_GetPathPoints::UBTTask_GetPathPoints(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance=true;
	NodeName="Get Path Points";
}

EBTNodeResult::Type UBTTask_GetPathPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard=OwnerComp.GetBlackboardComponent();
	AAIController* MyController=OwnerComp.GetAIOwner();
	if(!MyController||!MyBlackboard)
	{
		return EBTNodeResult::Failed;
	}

	AShooterBot* Bot=Cast<AShooterBot>(MyController->GetPawn());
	if(!Bot)
	{
		return EBTNodeResult::Failed;
	}

	APatrolPath* PathRef=Cast<APatrolPath>(Bot->SmartObject);
	if(!PathRef||PathRef->Locations.Num()<1)
	{
		return EBTNodeResult::Succeeded;
	}

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation",PathRef->Locations[Index]);

	if(Index<PathRef->Locations.Num()-1)
	{
		Index++;
		return EBTNodeResult::Succeeded;
	}

	Index=0;
	return EBTNodeResult::Succeeded;
	
}
