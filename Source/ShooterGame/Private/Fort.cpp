// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Fort.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/InputComponent.h" 
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Weapons/ShooterProjectile.h"

// Sets default values
AFort::AFort()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TurnRate = 90.0f;
	LookUpRate = 90.0f;

	MaxPitchAngle=45.0f;
	MinPitchAngle=10.0f;
	MaxYawAngle=45.0f;

	GunLength=290.0f;
	
	TriggerBox=CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Zone"));
	TriggerBox->SetWorldScale3D(FVector(1.15f,1.15f,1.15f));
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent=TriggerBox;
	
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this,&AFort::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this,&AFort::OnOverlapEnd);

	Fort = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Fort"));
	Fort->bReceivesDecals=false;
	Fort->CastShadow=false;
	Fort->SetupAttachment(TriggerBox);

	FollowCamera=CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetRelativeRotation(FRotator(20.0f,0,0));
	FollowCamera->SetupAttachment(Fort);
	
	LookDown = CreateDefaultSubobject<UChildActorComponent>(TEXT("Look Down Camera"));
	LookDown->SetupAttachment(TriggerBox);
	bIsLookDown=false;
	
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(Fort);
	MuzzleLocation->SetRelativeLocation(FVector(15.0f,0.0f,81.0f));
	
}

// Called when the game starts or when spawned
void AFort::BeginPlay()
{
	Super::BeginPlay();

	World=GetWorld();
}

// Called every frame
void AFort::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFort::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("PossessFort",IE_Pressed,this,&AFort::PossessIt);
	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&AFort::OnFire);
	PlayerInputComponent->BindAction("FortCameraChange",IE_Pressed,this,&AFort::ChangeCamera);

	PlayerInputComponent->BindAxis("LookUp",this,&AFort::LookAtRate);
	PlayerInputComponent->BindAxis("Turn",this,&AFort::TurnAtRate);
}

void AFort::OnFire()
{
	
	//参考ShooterWeapon_Projectile::FireWeapon()实现，无法发射，同时引擎崩溃，猜测原因为Fort并非继承于ShooterWeapon
	// FVector ShootDir = GetMuzzleLocation()->GetComponentRotation().Vector();
	// FVector Origin = GetMuzzleLocation()->GetComponentLocation();
	//
	// FTransform SpawnTM(ShootDir.Rotation(),Origin);
	// AShooterProjectile* Projectile=Cast<AShooterProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this,NULL,SpawnTM));
	// if(Projectile)
	// {
	// 	Projectile->SetInstigator(GetInstigator());
	// 	Projectile->SetOwner(this);
	// 	Projectile->InitVelocity(ShootDir);
	//
	// 	UGameplayStatics::FinishSpawningActor(Projectile,SpawnTM);
	// }

	if(World!=NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,FString::Printf(TEXT("OnFire!")));
		SpawnRotation=FollowCamera->GetRelativeRotation();
		SpawnLocation=GetMuzzleLocation()->GetComponentLocation() + GunLength*SpawnRotation.Vector();
		
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		if(Particle!=NULL)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Particle,FTransform(SpawnRotation,SpawnLocation));
		}

		//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,GetController()->GetName());    //ShooterPlayerController，说明原来的ShooterCharacter没有Controller控制
		
		AShooterPlayerController* MyController=Cast<AShooterPlayerController>(GetController());
		
		//利用一个实例指针调整炮弹发射方向
		Projectile_Instance = World->SpawnActor<AShooterProjectile>(Projectile,SpawnLocation,SpawnRotation,ActorSpawnParams);
		Projectile_Instance->InitGravity(0.5f);
		if(NewPawn)
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,TEXT("NewPawn"));
			Projectile_Instance->SetInstigator(GetInstigator());
			Projectile_Instance->SetOwner(this);
			Projectile_Instance->SetController(MyController);
		}
		
		
		ForceSetProjectileMovementRotation(Projectile_Instance,SpawnRotation.Vector());
	}
}

void AFort::TurnAtRate(float Rate)
{
	FollowCamera->AddRelativeRotation(LimitRotationOffset(FRotator(0,Rate*TurnRate*GetWorld()->GetDeltaSeconds(),0)));
	//AddControllerYawInput(Rate*TurnRate*GetWorld()->GetDeltaSeconds());
}

void AFort::LookAtRate(float Rate)
{
	FollowCamera->AddRelativeRotation(LimitRotationOffset(FRotator(-Rate*LookUpRate*GetWorld()->GetDeltaSeconds(),0,0)));
	//AddControllerPitchInput(Rate*LookUpRate*GetWorld()->GetDeltaSeconds());
}

FRotator AFort::LimitRotationOffset(FRotator InRotator)
{
	float NextFramePitchValue=FollowCamera->GetRelativeRotation().Pitch+InRotator.Pitch;
	float NextFrameYawValue=FollowCamera->GetRelativeRotation().Yaw+InRotator.Yaw;
	if(NextFramePitchValue < MinPitchAngle || NextFramePitchValue > MaxPitchAngle || NextFrameYawValue > MaxYawAngle || NextFrameYawValue < -MaxYawAngle)
	{
		return FRotator::ZeroRotator;
	}
	else return InRotator;
}

void AFort::ChangeCamera()
{
	OurPlayerController=UGameplayStatics::GetPlayerController(this,0);
	if(OurPlayerController)
	{
		if(!bIsLookDown)    //切换为俯视视角
		{
			OurPlayerController->SetViewTargetWithBlend(LookDown->GetChildActor(),1.0f);
			bIsLookDown=true;
		}
		else
		{
			OurPlayerController->SetViewTargetWithBlend(this,1.0f);
			bIsLookDown=false;
		}
	}
}

void AFort::ForceSetProjectileMovementRotation(AShooterProjectile* Projectile, FVector_NetQuantizeNormal ShootDir)
{
	Projectile->InitVelocity(ShootDir);
}

inline USceneComponent* AFort::GetMuzzleLocation()
{
	return this->MuzzleLocation;
}


void AFort::DealDamage(float DamageAmount)
{
	
}

void AFort::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if(GEngine)
	{
		//DrawDebugString(GetWorld(),OtherActor->GetActorLocation(),"Press E to Take Control",nullptr,FColor::Green,5.0f,true);
		NewPawn=Cast<AShooterCharacter>(OtherActor);
		if(NewPawn)
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,TEXT("Catch ShooterCharacter"));
			
		}
	}
}

void AFort::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex)
{
	if(GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,FString::Printf(TEXT("Left ShooterCharacter")));
		//NewPawn=nullptr;
	}
}

void AFort::PossessIt()
{
	OurPlayerController=UGameplayStatics::GetPlayerController(this,0);
	AShooterPlayerController* MyController=Cast<AShooterPlayerController>(OurPlayerController);
	MyController->SetPossessFort(false);
	
	//OurPlayerController->UnPossess();
	if(OurPlayerController)
	{
		OurPlayerController->Possess(NewPawn);
	}
	bIsLookDown=false;
	//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,TEXT("Possess ShooterCharacter"));
}

