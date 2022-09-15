// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/BulletProjectile.h"

#include "Camera/CameraComponent.h"
#include "CameraShooter/MainPlayerController.h"
#include "CameraShooter/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CombatComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/Weapon.h"

ABulletProjectile::ABulletProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);

	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// Blocking only Visibility and WorldStatic
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = InitialSpeed;


	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
}
#if WITH_EDITOR
void ABulletProjectile::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ABulletProjectile, InitialSpeed))
	{
		ProjectileMovementComponent->InitialSpeed = InitialSpeed;
		ProjectileMovementComponent->MaxSpeed = InitialSpeed;
	}
}
#endif

void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();
	StartDestroyTimer();
	if(CollisionBox)
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &ABulletProjectile::OnHit);
	}
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &ABulletProjectile::OnBounce);
	}
}

void ABulletProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	const APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter> (UGameplayStatics::GetPlayerPawn(this, 0));
	if(OwnerCharacter)
	{
		AMainPlayerController* OwnerController = Cast<AMainPlayerController>(OwnerCharacter->Controller);
		if(OwnerController)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
			if (OwnerController->CameraType == ECameraType::FirstPerson)
			{
				Destroy();
			}
		}
	}
}

void ABulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABulletProjectile::Destroyed()
{
	Super::Destroyed();
}

void ABulletProjectile::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ABulletProjectile::DestroyTimerFinished, DestroyTime);
}
void ABulletProjectile::DestroyTimerFinished()
{
	if (this != nullptr)
	{
		Destroy();
	}
}

void ABulletProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (bDoOnce)
	{
		const APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter> (UGameplayStatics::GetPlayerPawn(this, 0));
		if (OwnerCharacter)
		{
			AMainPlayerController* OwnerController = Cast<AMainPlayerController>(OwnerCharacter->Controller);
			if (OwnerController)
			{
				if (OwnerController->bIsShowingTrajectory)
				{
					FPredictProjectilePathParams PathParams2;
					PathParams2.StartLocation = ImpactResult.Location;
					PathParams2.bTraceWithChannel = true; // tracing with a particle trace channel
					PathParams2.bTraceWithCollision = true;
	
					FVector ReflectionVector = UKismetMathLibrary::GetReflectionVector(
					UKismetMathLibrary::GetDirectionUnitVector(ImpactResult.TraceStart, ImpactResult.TraceEnd), ImpactResult.Normal);
		
					PathParams2.LaunchVelocity = ReflectionVector * InitialSpeed / 2.f;
		
					PathParams2.MaxSimTime = 7.f;
					PathParams2.ProjectileRadius = 5.f;
					PathParams2.SimFrequency = 30.f;
					PathParams2.TraceChannel = ECollisionChannel::ECC_Visibility;
					PathParams2.ActorsToIgnore.Add(this);
		
					FPredictProjectilePathResult PathResult2;
		
					UGameplayStatics::PredictProjectilePath(this, PathParams2, PathResult2);
					
					if (OwnerCharacter->CombatComponent == nullptr || OwnerCharacter->CombatComponent->EquippedWeapon == nullptr) return;
					
					UWorld* World = GetWorld();
					if (World == nullptr) return;
					
					if(IsValid(OwnerCharacter->CombatComponent->EquippedWeapon->PathProjectileClass))
					{
						for (auto PathPoint2 : PathResult2.PathData)
						{
							AActor* TrajectoryActor2 = World->SpawnActor<AActor>(OwnerCharacter->CombatComponent->EquippedWeapon->PathProjectileClass,
								PathPoint2.Location, ReflectionVector.Rotation());
							if(TrajectoryActor2)
							{
								OwnerCharacter->CombatComponent->EquippedWeapon->TrajectoryActorArray2.Add(TrajectoryActor2);
							}
						}
					}
					bDoOnce = false;
				}
			}
		}
	}

}
