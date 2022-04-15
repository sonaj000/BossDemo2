// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/RangedOrb.h"
#include "BigBox.generated.h"

/**
 * 
 */
class UProjectileMovementComponent;
UCLASS()
class REFINED_API ABigBox : public ARangedOrb
{
	GENERATED_BODY()
public:
	ABigBox();

	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* ProjectileMove;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void LaunchBox();

	UPROPERTY(EditAnywhere)
		float InitialS = 500.0f;

	UPROPERTY(EditAnywhere)
		float DelayMove;
protected:

	virtual void BeginPlay() override;
	
};
