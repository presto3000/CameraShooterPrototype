// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CombatComponent.h"
#include "UI/MyHUD.h"
#include "UI/OverviewPlayerWidget.h"


void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Aim", IE_Pressed, this, &AMainPlayerController::OnAimButtonPressed);
	InputComponent->BindAction("Aim", IE_Released, this, &AMainPlayerController::OnAimButtonReleased);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AMainPlayerController::OnFireButtonPressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AMainPlayerController::OnFireButtonReleased);
	InputComponent->BindAction("ZoomInCamera", IE_Pressed, this, &AMainPlayerController::OnZoomInCameraPressed);
	InputComponent->BindAction("ZoomOutCamera", IE_Pressed, this, &AMainPlayerController::OnZoomOutCameraPressed);
	InputComponent->BindAction("ShowProjectileTrajectory", IE_Pressed, this, &AMainPlayerController::OnShowProjectileTracjectoryPressed);
	InputComponent->BindAction("ShowProjectileTrajectory", IE_Released, this, &AMainPlayerController::OnShowProjectileTracjectoryReleased);
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PlayerCharacter = Cast<APlayerCharacter>(InPawn);
}
void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = PlayerHUD == nullptr ? Cast<AMyHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD)
	{
		if(PlayerHUD->MainPlayerOverlayRef == nullptr) PlayerHUD->AddCharacterOverlay();
		SetHUDScore(Score);
	}
}

void AMainPlayerController::OnAimButtonPressed()
{
	if (PlayerCharacter == nullptr || PlayerCharacter->CombatComponent == nullptr) return;
	
	//CombatComponent->bAiming = true;
	PlayerCharacter->CombatComponent->SetAiming(true);
	PlayerCharacter->OnAimButtonPressedBP();
	
	if (CameraType == ECameraType::FirstPerson)
	{
		CameraType = ECameraType::ThirdPerson;
		PlayerCharacter->FirstPersonCamera->SetActive(false, false);
		PlayerCharacter->ThirdPersonCamera->SetActive(true, false);
		PlayerCharacter->GetMesh()->SetVisibility(true);
	}
	else if (CameraType == ECameraType::ThirdPerson)
	{
		CameraType = ECameraType::FirstPerson;
		PlayerCharacter->FirstPersonCamera->SetActive(true, false);
		PlayerCharacter->ThirdPersonCamera->SetActive(false, false);
		PlayerCharacter->GetMesh()->SetVisibility(false);
	}
}

void AMainPlayerController::OnAimButtonReleased()
{
	if (PlayerCharacter == nullptr || PlayerCharacter->CombatComponent == nullptr) return;
	
	//CombatComponent->bAiming = false;
	PlayerCharacter->CombatComponent->SetAiming(false);
	PlayerCharacter->OnAimButtonReleasedBP();
}

void AMainPlayerController::OnFireButtonPressed()
{
	if (PlayerCharacter == nullptr || PlayerCharacter->CombatComponent == nullptr) return;
	
	PlayerCharacter->CombatComponent->FireButtonPressed(true);
	PlayerCharacter->OnAimButtonPressedBP();
}

void AMainPlayerController::OnFireButtonReleased()
{
	if (PlayerCharacter == nullptr || PlayerCharacter->CombatComponent == nullptr) return;

	PlayerCharacter->CombatComponent->FireButtonPressed(false);
}

void AMainPlayerController::OnZoomInCameraPressed()
{
	if (PlayerCharacter == nullptr ||  PlayerCharacter->FirstPersonCamera == nullptr) return;
	PlayerCharacter->FirstPersonCamera->FieldOfView = FMath::Clamp(PlayerCharacter->FirstPersonCamera->FieldOfView + 10.f, 20.f, 90.f);
}

void AMainPlayerController::OnZoomOutCameraPressed()
{
	if (PlayerCharacter == nullptr ||  PlayerCharacter->FirstPersonCamera == nullptr) return;
	PlayerCharacter->FirstPersonCamera->FieldOfView = FMath::Clamp(PlayerCharacter->FirstPersonCamera->FieldOfView - 10.f, 20.f, 90.f);
}

void AMainPlayerController::OnShowProjectileTracjectoryPressed()
{
	if (CameraType == ECameraType::FirstPerson)
	{
		// TODO:: Show Projectile Path
	}
}

void AMainPlayerController::OnShowProjectileTracjectoryReleased()
{
	// TODO:: Hide Projectile Path
}

void AMainPlayerController::SetHUDScore(int32 ScoreValue)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<AMyHUD>(GetHUD()) : PlayerHUD;
	const bool bHUDValid = PlayerHUD && PlayerHUD->MainPlayerOverlayRef && PlayerHUD->MainPlayerOverlayRef->TxtScore;
	if(bHUDValid)
	{
		const FString ScoreText = FString::Printf(TEXT("%d"), ScoreValue);
		PlayerHUD->MainPlayerOverlayRef->TxtScore->SetText(FText::FromString(ScoreText));
	}
}
