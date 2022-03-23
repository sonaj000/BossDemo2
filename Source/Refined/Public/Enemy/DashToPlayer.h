// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"

#include "DashToPlayer.generated.h"

/**
 * 
 */
class UNavigationSystemV1;

UCLASS()
class REFINED_API UDashToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UDashToPlayer(FObjectInitializer const& objectIntializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory) override;

	UPROPERTY(VisibleAnywhere)
		UNavigationSystemV1* NavArea;

	UPROPERTY(VisibleAnywhere, Category = "Search")
		FVector RandomLocation;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Search")
		float search_radius = 1500.0f;


	
};
