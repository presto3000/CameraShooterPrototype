// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "CameraShooter/MainPlayerController.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/BulletProjectile.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);
	
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::Fire(const FVector& HitTarget)
{
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	
	UWorld* World = GetWorld();
	
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	
	if(MuzzleFlashSocket && World)
	{
		// From muzzle flash socket to hit location from TraceUnderCrosshairs
		SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		const FRotator TargetRotation = ToTarget.Rotation();

		FActorSpawnParameters SpawnParameters;
		// Character owning the weapon
		SpawnParameters.Owner = GetOwner();
		SpawnParameters.Instigator = InstigatorPawn;
		
		if (IsValid(ProjectileClass))
		{
			SpawnedProjectile = World->SpawnActor<ABulletProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParameters);
			
			PC = PC == nullptr ? Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : PC;
			
			// Activate BulletCam if We are in 1stPerson Mode
			if(PC && PC->CameraType == ECameraType::FirstPerson)
			{
				PC->SetViewTargetWithBlend(SpawnedProjectile, 0.3);
			}
		}
	}
}

void AWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AWeapon::ShowProjectileTrajectory(const FVector& HitTarget)
{
	UWorld* World = GetWorld();
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

	if(MuzzleFlashSocket && World)
	{
		// From muzzle flash socket to hit location from TraceUnderCrosshairs
		const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		const FRotator TargetRotation = ToTarget.Rotation();
		SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FActorSpawnParameters SpawnParameters;
		// Character owning the weapon
		SpawnParameters.Owner = GetOwner();
		SpawnParameters.Instigator = InstigatorPawn;
		if (IsValid(FakeProjectileClass))
		{
			ABulletProjectile* FakeProjectile = World->SpawnActor<ABulletProjectile>(FakeProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParameters);
			if (FakeProjectile == nullptr) return;
			FPredictProjectilePathParams PathParams;
			PathParams.bTraceWithChannel = true; // tracing with a particle trace channel
			PathParams.bTraceWithCollision = true;
			PathParams.LaunchVelocity = FakeProjectile->GetActorForwardVector() * BulletInitialSpeed;
			PathParams.MaxSimTime = 7.f;
			PathParams.ProjectileRadius = 5.f;
			PathParams.SimFrequency = 30.f;
			// MuzzleFlashSocket Start
			PathParams.StartLocation = FakeProjectile->GetActorLocation();
			PathParams.TraceChannel = ECollisionChannel::ECC_Visibility;
			PathParams.ActorsToIgnore.Add(FakeProjectile);
			
			UGameplayStatics::PredictProjectilePath(FakeProjectile, PathParams, PathResult);
			FakeProjectile->bDoOnce = true;
			
			// Draw Trajectory Path on the Screen by spawning Actor in PathPoint location in PathData
			for (auto PathPoint : PathResult.PathData)
			{
				if(IsValid(PathProjectileClass))
				{
					AActor* TrajectoryActor = World->SpawnActor<AActor>(PathProjectileClass, PathPoint.Location, TargetRotation);
					if(TrajectoryActor)
					{
						TrajectoryActorArray.Add(TrajectoryActor);
					}
				}
			}
		}
	}
}
void AWeapon::HideProjectileTrajectory()
{
	for (const auto Actor : TrajectoryActorArray)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
	for (const auto Actor : TrajectoryActorArray2)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}



