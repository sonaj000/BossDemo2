// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/RangedOrb.h"
#include "CloseBox.generated.h"

/**
 * 
 */
UCLASS()
class REFINED_API ACloseBox : public ARangedOrb
{
	GENERATED_BODY()
public:
	ACloseBox();

	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void Move();

	UPROPERTY(EditAnywhere)
		float Delay;

	UPROPERTY(BlueprintReadOnly)
		bool bCanMove;

	virtual void BeginOverLap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
		void D();
};
