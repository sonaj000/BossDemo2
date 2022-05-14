// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/RangedOrb.h"
#include "DelayedOrb.generated.h"

/**
 * 
 */
class UDamage;

UCLASS()
class REFINED_API ADelayedOrb : public ARangedOrb
{
	GENERATED_BODY()
public:
	// Called every frame
	ADelayedOrb();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginOverLap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
protected:
	UPROPERTY()
		bool bcanDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType>DelayedOrbDamage;
};
