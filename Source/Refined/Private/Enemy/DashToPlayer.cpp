// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DashToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Enemy/RangedOrb.h"
#include "Enemy/Boss_Controller.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enemy/Enemy_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

UDashToPlayer::UDashToPlayer(FObjectInitializer const& objectIntializer)
{
}

EBTNodeResult::Type UDashToPlayer::ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory)
{
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	ABoss_Controller* Boss = Cast<ABoss_Controller>(owner.GetAIOwner());
	AEnemy_Base* B = Cast<AEnemy_Base>(owner.GetAIOwner()->GetPawn());

	if (Boss)
	{
		Boss->MoveTo(Boss->Holder->GetActorLocation());
		//RandomLocation = FVector(29000, -847.0f, 180.0f);
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	owner.GetBlackboardComponent()->SetValueAsVector(FName("RandomLoc"), RandomLocation);
	UE_LOG(LogTemp, Warning, TEXT("the blackboard key is set"));
	return EBTNodeResult::Succeeded;
}
