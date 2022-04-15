// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Teleport_Task.generated.h"

/**
 * 
 */
class UNavigationSystemV1;
class UNiagaraComponent;
class UNiagaraFunctionLibrary;
class UNiagaraSystem;

UCLASS(Blueprintable)
class REFINED_API UTeleport_Task : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UTeleport_Task();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory) override;

	UPROPERTY(VisibleAnywhere)
		UNavigationSystemV1* NavArea;

	UPROPERTY(EditAnywhere)
		float SearchRadius; 

protected:

	UPROPERTY(VisibleAnywhere, Category = "Search")
		FVector TPLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UNiagaraSystem* TPEffect;
	
};
