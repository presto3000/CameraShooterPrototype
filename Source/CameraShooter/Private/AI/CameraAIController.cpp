// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CameraAIController.h"

#include "AI/CameraAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ACameraAIController::ACameraAIController()
{
	BlackboardComponent2 = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent2"));
	//check is valid
	check(BlackboardComponent2);

	BehaviorTreeComponent2 = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent2"));
	check(BehaviorTreeComponent2);
}

void ACameraAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(InPawn == nullptr) return;
    
	const ACameraAI* EnemyCam = Cast<ACameraAI>(InPawn);
	if(EnemyCam)
	{
		if(EnemyCam->GetBehaviorTree())
		{
			// Initialize Blackboard
			BlackboardComponent2->InitializeBlackboard(*(EnemyCam->GetBehaviorTree()->BlackboardAsset));
		}
	}
}
