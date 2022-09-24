// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGame.h"
#include "Bots/ShooterBot.h"
#include "Bots/ShooterAIController.h"
#include "PatrolPath.h"
#include "SmartObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

AShooterBot::AShooterBot(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AIControllerClass = AShooterAIController::StaticClass();

	UpdatePawnMeshes();

	bUseControllerRotationYaw = true;

	bPatrol=false;

	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	
	SightConfig->SightRadius = SightSenseRadius;    //初始视野半径（警戒区域）
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 200.f;   //失去视野半径
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;   //视角
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(0.1f);     //被刺激后经过时间的遗忘设置（0表示永不）

	AIPerComp->ConfigureSense(*SightConfig);
	AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());     //设置视觉感知为优先主导
	AIPerComp->OnPerceptionUpdated.AddDynamic(this, &AShooterBot::OnSensed);     //感知到时执行OnSensed()
}

bool AShooterBot::IsFirstPerson() const
{
	return false;
}

void AShooterBot::FaceRotation(FRotator NewRotation, float DeltaTime)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 8.0f);

	Super::FaceRotation(CurrentRotation, DeltaTime);
}

bool AShooterBot::IsbPatrol()
{
	return bPatrol;
}

void AShooterBot::OnSensed(const TArray<AActor*>& UpdatedActors)
{
	AShooterAIController* BotController=Cast<AShooterAIController>(GetController());
	
	for(int i=0;i<UpdatedActors.Num();i++)
	{
		FActorPerceptionBlueprintInfo Info;
		AIPerComp->GetActorsPerception(UpdatedActors[i], Info);

		AShooterCharacter* Chr=Cast<AShooterCharacter>(UpdatedActors[i]);
		
		if (Chr&&Info.LastSensedStimuli[0].WasSuccessfullySensed())     //感知到敌人
		{
			if(BotController)
			{
				//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,TEXT("Sense Enemy"));
				BotController->GetBlackboardComp()->SetValue<UBlackboardKeyType_Bool>(FName("SenseEnemy"),true);
			}
		}
		else
		{
			if(BotController)
			{
				//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,TEXT("Not Sense Enemy"));
				BotController->GetBlackboardComp()->SetValue<UBlackboardKeyType_Bool>(FName("SenseEnemy"),false);
			}
		}
	}
}

ASmartObject* AShooterBot::GetSmartObject()
{
	return SmartObject;
}
