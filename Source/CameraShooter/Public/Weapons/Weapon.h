// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
