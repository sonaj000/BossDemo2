// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Boss_Controller.generated.h"

/**
 * 
 */
class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class REFINED_API ABoss_Controller : public AAIController
{
	GENERATED_BODY()
public:

	ABoss_Controller(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* inPawn) override;

	UPROPERTY(Transient)
		UBehaviorTreeComponent* BTC;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
		UBehaviorTree* bTree;

	UPROPERTY(Transient)
		UBlackboardComponent* BBC;

	uint8 EnemyKeyID;

	UBlackboardComponent* get_blackboard() const;

	//Perception setup
	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>& DetectedPawns);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float BossSightRadius = 2000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float BossSightAge = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float BossLoseSight = BossSightRadius + 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float BossFieldofView = 360.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool bisCharacterDetected;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float DistanceToPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		AActor* Holder = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* TestMontage;


protected:
	
	


	
};
