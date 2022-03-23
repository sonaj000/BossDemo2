// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DashToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

UDashToPlayer::UDashToPlayer(FObjectInitializer const& objectIntializer)
{
}

EBTNodeResult::Type UDashToPlayer::ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory)
{
	NavArea = FNavigationSystem::
		GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (NavArea)
	{
		NavArea->K2_GetRandomReachablePointInRadius(GetWorld(),
		GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(),RandomLocation, 15000.0f);
		UE_LOG(LogTemp, Warning, TEXT("random location acquired: %s"), *RandomLocation.ToString());
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
