// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BeingHit_Dec.generated.h"

/**
 * 
 */
class UHealthComp;
UCLASS(meta = (DisplayName = "Is being Hit"))
class REFINED_API UBeingHit_Dec : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UHealthComp>DaHealth;
	
};
