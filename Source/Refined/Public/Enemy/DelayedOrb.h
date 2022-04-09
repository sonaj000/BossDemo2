// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/RangedOrb.h"
#include "DelayedOrb.generated.h"

/**
 * 
 */
UCLASS()
class REFINED_API ADelayedOrb : public ARangedOrb
{
	GENERATED_BODY()
public:
	// Called every frame
	ADelayedOrb();

	virtual void Tick(float DeltaTime) override;
	
};
