// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CombatComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/Weapon.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; 
	CameraBoom->bUsePawnControlRotation = true;

	// ThirdPersonCamera
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false; 

	
	CameraBoomFirstPerson = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoomFirstPerson"));
	CameraBoomFirstPerson->SetupAttachment(RootComponent);
	CameraBoomFirstPerson->TargetArmLength = 100.f; 
	CameraBoomFirstPerson->bUsePawnControlRotation = true;
	
	// FirstPersonCamera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(CameraBoomFirstPerson, USpringArmComponent::SocketName);
	FirstPersonCamera->bUsePawnControlRotation = false;
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent)
	{
		CombatComponent->Character = this;
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		Destroy();

		UWorld* World = GetWorld();
		if(World)
		{
			UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));
		}
	}
	else
	{
		Health -= DamageAmount;
	}
	return DamageAmount;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultWeapon();
}

void APlayerCharacter::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
    if (World && IsValid(DefaultWeaponClass))
    {
    	AWeapon* StartingWeapon = World->SpawnActor<AWeapon>(DefaultWeaponClass);
    	if (CombatComponent)
    	{
    		CombatComponent->EquipWeapon(StartingWeapon);
    	}
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



