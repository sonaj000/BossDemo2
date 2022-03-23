// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedOrb.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraFunctionLibrary;

UCLASS()
class REFINED_API ARangedOrb : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	ARangedOrb();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UStaticMeshComponent* Orb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		USphereComponent* HitBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UNiagaraSystem* OrbExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UNiagaraComponent* Quick;
	UPROPERTY()
		UNiagaraFunctionLibrary* Test;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
