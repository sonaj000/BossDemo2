// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BossClose_Dec.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "BossClose"))
class REFINED_API UBossClose_Dec : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
		float Distance;
};
