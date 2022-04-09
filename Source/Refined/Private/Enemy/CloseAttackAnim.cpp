// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CloseAttackAnim.h"
#include "Enemy/Boss_Controller.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enemy/Enemy_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Enemy/RangedOrb.h"


EBTNodeResult::Type UCloseAttackAnim::ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory)
{
	ABoss_Controller* Boss = Cast<ABoss_Controller>(owner.GetAIOwner());
	B = Cast<AEnemy_Base>(owner.GetAIOwner()->GetPawn());
	if (B)
	{
		B->GetCharacterMovement()->MaxWalkSpeed = 1000;
		B->CloseAttack();

		FActorSpawnParameters SpawnerParams;
		SpawnerParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Front = B->GetActorLocation() + B->GetActorForwardVector()*200 + FVector(0,0,50.0f);

		ARangedOrb* Mat = GetWorld()->SpawnActor<ARangedOrb>(CloseCube, Front, B->GetActorRotation(), SpawnerParams);
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	owner.GetBlackboardComponent()->SetValueAsClass(FName("B"),T);
	return EBTNodeResult::Succeeded;
}

