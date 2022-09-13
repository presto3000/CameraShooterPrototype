// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "OverviewPlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAMERASHOOTER_API UOverviewPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TxtScore;
};
