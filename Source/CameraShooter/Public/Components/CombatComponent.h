// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/MyHUD.h"
#include "CombatComponent.generated.h"

class AWeapon;
class AMainPlayerController;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMERASHOOTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	void SetAiming(bool bIsAiming);
	void FireButtonPressed(bool bPressed);
	void EquipWeapon(AWeapon* WeaponToEquip);
	void AttachActorToRightHand(AActor* ActorToAttach);
protected:
	virtual void BeginPlay() override;
	bool bAiming = false;
	bool bFireButtonPressed;
	void Fire();
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	/************************************************
					HandyRefs
	*************************************************/
	UPROPERTY(VisibleAnywhere, Category = HandyRefs)
	APlayerCharacter* Character;
	UPROPERTY(VisibleAnywhere, Category = HandyRefs)
	AMainPlayerController* PC;
	UPROPERTY(VisibleAnywhere, Category = HandyRefs)
	AMyHUD* HUD;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AWeapon* EquippedWeapon;

	/************************************************
					Crosshairs
	*************************************************/
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	void SetHUDCrosshairs(float DeltaTime);
	FHUDCrosshairStruct HUDCrosshairStruct;

	FVector HitTarget;
};
