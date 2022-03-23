// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CloseAttackAnim.generated.h"

/**
 * 
 */
class AEnemy_Base;
UCLASS(Blueprintable)
class REFINED_API UCloseAttackAnim : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory) override;

	UPROPERTY(VisibleAnywhere)
		AEnemy_Base* B;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AEnemy_Base>T;
};
