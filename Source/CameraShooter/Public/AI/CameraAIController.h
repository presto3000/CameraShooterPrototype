// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CameraAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class CAMERASHOOTER_API ACameraAIController : public AAIController
{
	GENERATED_BODY()
public:
	ACameraAIController();
	virtual void OnPossess(APawn* InPawn) override;

private:
	/** Blackboard component for this enemy */
	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent2;
    
	/** Behavior tree component for this enemy */
	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComponent2;
    
public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComponentFromEnemyController() const  {return BlackboardComponent2; }
};
