// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CheckDistance_Dec.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enemy/Enemy_Base.h"
#include "Enemy/Boss_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MCharacter.h"

bool UCheckDistance_Dec::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	AEnemy_Base* AIPawn = Cast<AEnemy_Base>(OwnerComp.GetAIOwner()->GetPawn());
	ABoss_Controller* test = Cast<ABoss_Controller>(OwnerComp.GetAIOwner());
	AMCharacter* Char = Cast<AMCharacter>(test->Holder);
	OwnerComp.GetBlackboardComponent()->SetValueAsClass(FName("MCharRef"), MainCharacter);
	if ((AIPawn->GetActorLocation() - Char->GetActorLocation()).Size() < 1000)
	{
		UE_LOG(LogTemp, Warning, TEXT("Distance is less than so true"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Distance is greater than so true"));
		return false;
	}

}

