// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CloseAttackAnim.h"
#include "Enemy/Boss_Controller.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enemy/Enemy_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

EBTNodeResult::Type UCloseAttackAnim::ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory)
{
	ABoss_Controller* Boss = Cast<ABoss_Controller>(owner.GetAIOwner());
	B = Cast<AEnemy_Base>(owner.GetAIOwner()->GetPawn());
	if (B)
	{
		B->GetCharacterMovement()->MaxWalkSpeed = 1000;
		B->CloseAttack();
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	owner.GetBlackboardComponent()->SetValueAsClass(FName("B"),T);
	return EBTNodeResult::Succeeded;
}

