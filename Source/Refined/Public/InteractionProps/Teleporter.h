// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Teleporter.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class REFINED_API ATeleporter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleporter();

public:

	UPROPERTY(EditAnywhere, Category = "Components")
		ATeleporter* ToTeleport;

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* TeleportSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* HitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector THere;
protected:
	// Called when the game starts or when spawned

	UFUNCTION()
		void OnOverLap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
