// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "BulletProjectile.generated.h"

class UCameraComponent;
class UProjectileMovementComponent;
class UBoxComponent;
UCLASS()
class CAMERASHOOTER_API ABulletProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletProjectile();

	UPROPERTY(EditAnywhere)
	float Damage = 100.f;

protected:
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif
	
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit);

	FPredictProjectilePathResult PathResult;
	
	void StartDestroyTimer();
	void DestroyTimerFinished();
	
	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	
	UPROPERTY(EditAnywhere)
	float InitialSpeed = 3000;

private:
	FTimerHandle DestroyTimer;
	UPROPERTY(EditAnywhere)
	float DestroyTime = 2.f;


};
