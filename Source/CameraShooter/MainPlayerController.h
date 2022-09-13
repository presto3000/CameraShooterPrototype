// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class AMyHUD;
class APlayerCharacter;
/**
 * 
 */
UENUM(BlueprintType)
enum class ECameraType : uint8
{
	ThirdPerson UMETA(DisplayName = "ThirdPerson"),
	FirstPerson UMETA(DisplayName = "FirstPerson"),
};

UCLASS()
class CAMERASHOOTER_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECameraType CameraType = ECameraType::ThirdPerson;

	int32 Score = 0.f;
	
	/********************************************************************
			Input callback functions
	********************************************************************/

	/************************************************
					 ACTIONS
	*************************************************/
	void OnAimButtonPressed();
	void OnAimButtonReleased();
	void OnFireButtonPressed();
	void OnFireButtonReleased();
	void OnZoomInCameraPressed();
	void OnZoomOutCameraPressed();
	void OnShowProjectileTracjectoryPressed();
	void OnShowProjectileTracjectoryReleased();
	void SetHUDScore(int32 ScoreValue);
	
	UPROPERTY()
	AMyHUD* PlayerHUD;
};
