// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RangedAttack.generated.h"

/**
 * 
 */
class UNiagaraSystem;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraFunctionLibrary;
class ARangedOrb;
UCLASS(Blueprintable)
class REFINED_API URangedAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
		FVector SpawnLoc;
	UFUNCTION()
	void SpawnAttack();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<ARangedOrb>RB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UNiagaraSystem* CircleIndicator;




};
