// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossClose_Dec.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enemy/Enemy_Base.h"
#include "Enemy/Boss_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MCharacter.h"

bool UBossClose_Dec::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	AEnemy_Base* AIPawn = Cast<AEnemy_Base>(OwnerComp.GetAIOwner()->GetPawn());
	ABoss_Controller* test = Cast<ABoss_Controller>(OwnerComp.GetAIOwner());
	AMCharacter* MChar = Cast<AMCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsClass(FName("MCharRef")));
	if (AIPawn) // - MChar->GetActorLocation()).Size()
	{
		return true;
	}
	else
	{
		return false;
	}


}