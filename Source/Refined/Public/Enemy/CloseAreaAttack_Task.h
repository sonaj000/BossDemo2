// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CloseAreaAttack_Task.generated.h"

/**
 * 
 */
class AEnemy_Base;
class UNiagaraSystem;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraFunctionLibrary;
class ARangedOrb;

UCLASS(Blueprintable)
class REFINED_API UCloseAreaAttack_Task : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory) override;

	UPROPERTY(VisibleAnywhere)
		AEnemy_Base* T;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<ARangedOrb>Explode;
	
};
