// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BigCube_Task.generated.h"

/**
 * 
 */
class ABigBox;
UCLASS(Blueprintable)
class REFINED_API UBigCube_Task : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBigCube_Task();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory) override;

protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABigBox>BB;
	
};
