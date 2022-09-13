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
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		const FRotator TargetRotation = ToTarget.Rotation();

		FActorSpawnParameters SpawnParameters;
		// Character owning the weapon
		SpawnParameters.Owner = GetOwner();
		SpawnParameters.Instigator = InstigatorPawn;

		ABulletProjectile* SpawnedProjectile = nullptr;
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



