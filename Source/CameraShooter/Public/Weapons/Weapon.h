// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectiles/BulletProjectile.h"
#include "Weapon.generated.h"

class AMainPlayerController;
class ABulletProjectile;

UCLASS()
class CAMERASHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	UPROPERTY(VisibleAnywhere, Category = HandyRefs)
	AMainPlayerController* PC;
	
	void Fire(const FVector& HitTarget);
	virtual void Tick(float DeltaSeconds) override;
	/************************************************
				 BulletTrajectory
	*************************************************/

	void ShowProjectileTrajectory(const FVector& HitTarget);
	void HideProjectileTrajectory();

	// Same as BulletProjectile
	float BulletInitialSpeed = 5000.f;
	bool bShowPath = false; 
	
	UPROPERTY()
	ABulletProjectile* SpawnedProjectile = nullptr;
	// MuzzleFlashSocket
	FTransform SocketTransform;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABulletProjectile> FakeProjectileClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PathProjectileClass;

	FPredictProjectilePathResult PathResult;
	UPROPERTY()
	TArray<AActor*> TrajectoryActorArray;
	UPROPERTY()
	TArray<AActor*> TrajectoryActorArray2;
	/************************************************
    				 Crosshair
    *************************************************/
    // Crosshair : Textures for the weapon crosshairs
    UPROPERTY(EditAnywhere, Category = Crosshairs)
    UTexture2D* CrosshairsCenter;
    UPROPERTY(EditAnywhere, Category = Crosshairs)
    UTexture2D* CrosshairsLeft;
    UPROPERTY(EditAnywhere, Category = Crosshairs)
    UTexture2D* CrosshairsRight;
    UPROPERTY(EditAnywhere, Category = Crosshairs)
    UTexture2D* CrosshairsBottom;
    UPROPERTY(EditAnywhere, Category = Crosshairs)
    UTexture2D* CrosshairsTop;

private:
	UPROPERTY(VisibleAnywhere, Category = WeaponProperties)
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABulletProjectile> ProjectileClass;

// Getters:
public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const {return WeaponMesh; }
};
