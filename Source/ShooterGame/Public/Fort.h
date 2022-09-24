// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterProjectile.h"
#include "GameFramework/Pawn.h"

#include "Fort.generated.h"


UCLASS()
class SHOOTERGAME_API AFort : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFort();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APlayerController* OurPlayerController;
	APawn* NewPawn;

	UPROPERTY(VisibleAnywhere,Category="Trigger Zone")
		class UBoxComponent* TriggerBox;    //������Χ��
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)   
		class UChildActorComponent* LookDown;    //�������ͷ

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera)
		class UCameraComponent* FollowCamera;      //�ڿ��ӽǸ�������ͷ
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Fort;       //����������̨ģ��   

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleLocation;   //��̨�ڿ�λ��

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float TurnRate;   //yaw�Ƕ�ֵ
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float LookUpRate;   //pitch�Ƕ�ֵ

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		float MinPitchAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		float MaxPitchAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		float MaxYawAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float GunLength;
	
protected:

	void TurnAtRate(float Rate);
	void LookAtRate(float Rate);
	FRotator LimitRotationOffset(FRotator InRotator);
	void ChangeCamera();

	bool bIsLookDown;

	void ForceSetProjectileMovementRotation(AShooterProjectile* Projectile,FVector_NetQuantizeNormal ShootDir);
	
public:

	UPROPERTY(EditDefaultsOnly,Category = GamePlay)
		TSubclassOf<class AShooterProjectile> Projectile;

	UPROPERTY(EditDefaultsOnly,Category=GamePlay)
		class UParticleSystem* Particle;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	// 	class USoundBase* FireSound;   //������Ч

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	// 	class UAnimMontage* FireAnimation;    //���𶯻�

	class UWorld* World;

	AShooterProjectile* Projectile_Instance;

	void OnFire();
	
	//�ڵ�����
	FVector SpawnLocation;
	FRotator SpawnRotation;

public:

	void DealDamage(float DamageAmount);

	USceneComponent* GetMuzzleLocation();

	AShooterWeapon* Weapon;

public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

	void PossessIt();
	
};


