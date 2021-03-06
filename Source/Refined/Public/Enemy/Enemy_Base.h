// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy_Base.generated.h"

class UAudioComponent;
class UBehaviorTree;
class UHealthComp;

UCLASS()
class REFINED_API AEnemy_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy_Base();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		UBehaviorTree* TreeAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* TestMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* RangeMontage;
	// Audio components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		UAudioComponent* CloseAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		UAudioComponent* CloseAreaAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
		UAudioComponent* RangedAudio;

	UFUNCTION()
		void DelayedAudio();

	UFUNCTION()
		void CloseAttack();
	UFUNCTION()
		void RangeAttack();
	UFUNCTION()
		void CloseAreaAttack();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UHealthComp* HealthBar;

	UFUNCTION()
		void OnHealthChanged(UHealthComp* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
