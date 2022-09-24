// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGame.h"
#include "Weapons/ShooterWeapon_Projectile.h"
#include "Weapons/ShooterProjectile.h"

AShooterWeapon_Projectile::AShooterWeapon_Projectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

//////////////////////////////////////////////////////////////////////////
// Weapon usage

void AShooterWeapon_Projectile::FireWeapon()
{
	FVector ShootDir = GetAdjustedAim();
	FVector Origin = GetMuzzleLocation();

	// trace from camera to check what's under crosshair 飞弹轨迹，查找命中结果HitResult
	const float ProjectileAdjustRange = 10000.0f;
	const FVector StartTrace = GetCameraDamageStartLocation(ShootDir);
	const FVector EndTrace = StartTrace + ShootDir * ProjectileAdjustRange;
	FHitResult Impact = WeaponTrace(StartTrace, EndTrace);
	
	// and adjust directions to hit that actor
	if (Impact.bBlockingHit)
	{
		const FVector AdjustedDir = (Impact.ImpactPoint - Origin).GetSafeNormal();
		bool bWeaponPenetration = false;

		const float DirectionDot = FVector::DotProduct(AdjustedDir, ShootDir);
		if (DirectionDot < 0.0f)
		{
			// shooting backwards = weapon is penetrating
			bWeaponPenetration = true;
		}
		else if (DirectionDot < 0.5f)
		{
			// check for weapon penetration if angle difference is big enough  如果角度足够大，检查武器穿透
			// raycast along weapon mesh to check if there's blocking hit      沿武器网格进行射线投射检查是否有阻挡命中

			FVector MuzzleStartTrace = Origin - GetMuzzleDirection() * 150.0f;
			FVector MuzzleEndTrace = Origin;
			FHitResult MuzzleImpact = WeaponTrace(MuzzleStartTrace, MuzzleEndTrace);

			if (MuzzleImpact.bBlockingHit)
			{
				bWeaponPenetration = true;
			}
		}

		if (bWeaponPenetration)
		{
			// spawn at crosshair position
			Origin = Impact.ImpactPoint - ShootDir * 10.0f;
		}
		else
		{
			// adjust direction to hit
			ShootDir = AdjustedDir;
		}
	}
	
	ServerFireProjectile(Origin, ShootDir);   //服务器上调用
}

bool AShooterWeapon_Projectile::ServerFireProjectile_Validate(FVector Origin, FVector_NetQuantizeNormal ShootDir)   //验证
{
	return true;
}

void AShooterWeapon_Projectile::ServerFireProjectile_Implementation(FVector Origin, FVector_NetQuantizeNormal ShootDir)  //实施
{
	FTransform SpawnTM(ShootDir.Rotation(), Origin);
	AShooterProjectile* Projectile = Cast<AShooterProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileConfig.ProjectileClass, SpawnTM));
	if (Projectile)
	{
		//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,GetInstigator()->GetName());
		Projectile->SetInstigator(GetInstigator());   //煽动者
		Projectile->SetOwner(this);
		Projectile->InitVelocity(ShootDir);

		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTM);
	}
}

void AShooterWeapon_Projectile::ApplyWeaponConfig(FProjectileWeaponData& Data)
{
	Data = ProjectileConfig;
}
