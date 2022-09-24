// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ShooterCharacter.h"
#include "ShooterBot.generated.h"

UCLASS()
class AShooterBot : public AShooterCharacter
{
	GENERATED_UCLASS_BODY()
	
	virtual bool IsFirstPerson() const override;

	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;

	bool IsbPatrol();
	
	UPROPERTY(EditInstanceOnly)
	bool bPatrol;

public:
	UPROPERTY(EditAnywhere, Category=Behavior)
	class UBehaviorTree* BotBehavior;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Behavior)
	class ASmartObject* SmartObject;

	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
	class UAIPerceptionComponent* AIPerComp;      //用于在AI感知系统中，刺激侦听器并收集已注册的刺激

	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
	class UAISenseConfig_Sight* SightConfig;      //视觉感知配置，即视觉刺激，为UAIPerceptionComponent提供服务

	UPROPERTY(EditAnywhere,Category=Behavior)
	float SightSenseRadius = 1000.0f;     //感知范围，初始为1000
	
	UFUNCTION()
	void OnSensed(const TArray<AActor*>& UpdatedActors);

	FORCEINLINE ASmartObject* GetSmartObject();
	
};