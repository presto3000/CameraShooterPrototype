// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/CameraAI.h"

#include "CameraShooter/Definitions.h"
#include "CameraShooter/MainPlayerController.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/BulletProjectile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CameraAIController.h"


ACameraAI::ACameraAI()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACameraAI::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = 100 / RateOfFire;
}

void ACameraAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACameraAI::Death()
{
	Destroy();
}

void ACameraAI::CameraStartFire()
{
	const float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ACameraAI::Fire, TimeBetweenShots, true, FirstDelay);
}

void ACameraAI::CameraStopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void ACameraAI::Fire()
{
	FVector EyeLocation;
	FRotator EyeRotation;
	this->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	const FVector ShotDirection = EyeRotation.Vector();

	const FVector TraceEnd = EyeLocation + (ShotDirection * TRACE_LENGTH);
	const FVector HitTarget = TraceEnd;
	
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		World = GetWorld();
	}
	const USkeletalMeshSocket* MuzzleFlashSocket = this->GetMesh()->GetSocketByName(FName("MuzzleFlash"));
	
	if(MuzzleFlashSocket && World)
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetMesh());
		const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		const FRotator TargetRotation = ToTarget.Rotation();
		
		if (IsValid(ProjectileClass))
		{
			const FActorSpawnParameters SpawnParameters;
			World->SpawnActor<ABulletProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParameters);
		}
	}
	LastFireTime = GetWorld()->TimeSeconds;
}

float ACameraAI::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		
		PC = PC == nullptr ? Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : PC;
		if (PC->CameraType == ECameraType::FirstPerson)
		{
			// Add pkt 5pkt
			PC->Score += 5;
			PC->SetHUDScore(PC->Score);
		}
		else
		{
			// Add 1pkt to Score
			PC->Score++;
			PC->SetHUDScore(PC->Score);
		}
		Death();
	}
	else
	{
		Health -= DamageAmount;
	}
	return  DamageAmount;
}

