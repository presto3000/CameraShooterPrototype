// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractWithCrosshairs.h"
#include "CameraAI.generated.h"

class AMainPlayerController;
class ABulletProjectile;
UCLASS()
class CAMERASHOOTER_API ACameraAI : public ACharacter, public IInteractWithCrosshairs
{
	GENERATED_BODY()

public:
	ACameraAI();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY()
	AMainPlayerController* PC;

	UFUNCTION()
	void Death();
	
	/***********************************
     *				AI Camera Fire
     ************************************/
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABulletProjectile> ProjectileClass;
    UFUNCTION(BlueprintCallable)
    void CameraStartFire();
    UFUNCTION(BlueprintCallable)
    void CameraStopFire();
	void Fire();
    FTimerHandle TimerHandle_TimeBetweenShots;
	
    float LastFireTime;
    float RateOfFire = 600.f;
	// Derived from RateOfFire
	float TimeBetweenShots;

private:
	/** Current Enemy Health */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Health = 100.f;
	/** Max Enemy Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;
	
	/*****************************************
	*				Behaviour Tree
	*****************************************/
	/** Behavior tree for the AI Character */
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;
	
public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const {return BehaviorTree; }
};
