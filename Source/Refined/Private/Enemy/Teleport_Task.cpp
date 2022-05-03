// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Teleport_Task.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Enemy/Boss_Controller.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enemy/Enemy_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


UTeleport_Task::UTeleport_Task()
{
	NodeName = "Teleport Task";
	SearchRadius = 1000.0f;
}

EBTNodeResult::Type UTeleport_Task::ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory)
{
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	ABoss_Controller* Boss = Cast<ABoss_Controller>(owner.GetAIOwner());
	AEnemy_Base* B = Cast<AEnemy_Base>(owner.GetAIOwner()->GetPawn());

	FVector Curr = B->GetActorLocation();
	if (NavArea)
	{
		TPLoc = NavArea->GetRandomReachablePointInRadius(this, NavArea->GetWorldBounds().GetCenter(), SearchRadius);
		UNiagaraComponent* Tele = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),TPEffect, Curr - FVector(0, 0, 50.0f), FRotator(0, 0, 0.0f), FVector(2.0, 2.0, 5.0f));
		B->SetActorLocation(TPLoc);
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Succeeded;
}


