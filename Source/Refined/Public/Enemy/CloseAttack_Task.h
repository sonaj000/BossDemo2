// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CloseAttack_Task.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class REFINED_API UCloseAttack_Task : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector CharLoc;

	
};
