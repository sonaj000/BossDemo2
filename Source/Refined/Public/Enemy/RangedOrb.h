// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedOrb.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UNiagaraFunctionLibrary;
class UBoxComponent;
class UDamage;

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
		UBoxComponent* HitBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UNiagaraSystem* OrbExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UNiagaraComponent* Quick;
	UPROPERTY()
		UNiagaraFunctionLibrary* Test;

	UPROPERTY(EditAnywhere)
		float LifeTime;

	UPROPERTY(EditAnywhere)
		float ExplosionDelay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void SpawnExplosion();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType>RangedDamage;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void BeginOverLap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void End();

};
