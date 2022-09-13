// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"
#include "CameraShooter/Definitions.h"
#include "CameraShooter/MainPlayerController.h"
#include "CameraShooter/PlayerCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Interfaces/InteractWithCrosshairs.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/Weapon.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FHitResult HitResult;
	// TraceUnderCrosshairs
	TraceUnderCrosshairs(HitResult);

	// Set Crosshairs
	SetHUDCrosshairs(DeltaTime);
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(Character == nullptr || WeaponToEquip == nullptr) return;
	EquippedWeapon = WeaponToEquip;
	AttachActorToRightHand(EquippedWeapon);
}
void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach)
{
	if(Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr) return;
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if(HandSocket)
	{
		HandSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
}


void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;

	if(bFireButtonPressed)
	{
		Fire();
	}
}


void UCombatComponent::Fire()
{
	if (EquippedWeapon && Character)
	{
		EquippedWeapon->Fire(HitTarget);
	}
}
	
void UCombatComponent::SetAiming(bool bIsAiming)
{
	if(Character == nullptr || EquippedWeapon == nullptr) return;
	bAiming = bIsAiming;
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation (ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	// 2D screen space coordiante into a 3D
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);
	
	if(bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		if(Character)
		{
			float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 50.f);
			//DrawDebugSphere(GetWorld(), Start, 16.f, 12, FColor::Red, false);
		}
		
		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;
		
		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		
		if(TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairs>())
		{
			HUDCrosshairStruct.Crosshairscolor = FLinearColor::Red;
		}
		else
		{
			HUDCrosshairStruct.Crosshairscolor = FLinearColor::White;
		}

		if(!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
			HitTarget = End;
		}
		// when success
		else
		{
			HitTarget = TraceHitResult.ImpactPoint;
		}
	}
}

void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if(Character == nullptr || Character->Controller == nullptr) return;

	PC = PC == nullptr ? Cast<AMainPlayerController>(Character->Controller) : PC;
	if(PC)
	{
		HUD = HUD == nullptr ? Cast<AMyHUD>(PC->GetHUD()) : HUD;
		if(HUD)
		{
			if(EquippedWeapon)
			{
				if (EquippedWeapon->CrosshairsCenter == nullptr || EquippedWeapon->CrosshairsLeft == nullptr ||
					EquippedWeapon->CrosshairsRight == nullptr || EquippedWeapon->CrosshairsBottom == nullptr ||
					EquippedWeapon->CrosshairsTop == nullptr) return;
				
				HUDCrosshairStruct.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDCrosshairStruct.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDCrosshairStruct.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				HUDCrosshairStruct.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
				HUDCrosshairStruct.CrosshairsTop = EquippedWeapon->CrosshairsTop;
				HUD->SetHUDCrosshairStruct(HUDCrosshairStruct);
			}
			else
			{
				HUDCrosshairStruct.CrosshairsCenter = nullptr;
				HUDCrosshairStruct.CrosshairsLeft = nullptr;
				HUDCrosshairStruct.CrosshairsRight = nullptr;
				HUDCrosshairStruct.CrosshairsBottom = nullptr;
				HUDCrosshairStruct.CrosshairsTop = nullptr;
				HUD->SetHUDCrosshairStruct(HUDCrosshairStruct);
			}
		}
	}

	
}