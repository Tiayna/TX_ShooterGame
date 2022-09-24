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
	class UAIPerceptionComponent* AIPerComp;      //������AI��֪ϵͳ�У��̼����������ռ���ע��Ĵ̼�

	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
	class UAISenseConfig_Sight* SightConfig;      //�Ӿ���֪���ã����Ӿ��̼���ΪUAIPerceptionComponent�ṩ����

	UPROPERTY(EditAnywhere,Category=Behavior)
	float SightSenseRadius = 1000.0f;     //��֪��Χ����ʼΪ1000
	
	UFUNCTION()
	void OnSensed(const TArray<AActor*>& UpdatedActors);

	FORCEINLINE ASmartObject* GetSmartObject();
	
};