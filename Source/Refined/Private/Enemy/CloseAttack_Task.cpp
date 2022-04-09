// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CloseAttack_Task.h"
#include "Enemy/RangedOrb.h"
#include "Enemy/Boss_Controller.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enemy/Enemy_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

EBTNodeResult::Type UCloseAttack_Task::ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory)
{
	ABoss_Controller* Boss = Cast<ABoss_Controller>(owner.GetAIOwner());
	AEnemy_Base* B = Cast<AEnemy_Base>(owner.GetAIOwner()->GetPawn());
	if (Boss)
	{
		CharLoc = Boss->Holder->GetActorLocation() + Boss->Holder->GetActorForwardVector()*5;
		FRotator Turn = UKismetMathLibrary::FindLookAtRotation(B->GetActorLocation(), CharLoc);
		B->SetActorRotation(Turn);
		B->GetCharacterMovement()->MaxWalkSpeed = 5000;
		//B->CloseAttack();
		//not working
		//move to and change movement speed
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	owner.GetBlackboardComponent()->SetValueAsVector(FName("CharLoc"), CharLoc);
	return EBTNodeResult::Succeeded;
}

