// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RangedAttack.h"
#include "Enemy/RangedOrb.h"
#include "Enemy/Boss_Controller.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enemy/Enemy_Base.h"
#include "Enemy/Boss_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


EBTNodeResult::Type URangedAttack::ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory)
{
	FTimerHandle Pause;
	ABoss_Controller* Boss = Cast<ABoss_Controller>(owner.GetAIOwner());
	AEnemy_Base* B = Cast<AEnemy_Base>(owner.GetAIOwner()->GetPawn());
	if (Boss->Holder && CircleIndicator)
	{
		B->RangeAttack();
		SpawnLoc = Boss->Holder->GetActorLocation();
		UNiagaraComponent* CircleThing = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CircleIndicator, FVector(SpawnLoc.X, SpawnLoc.Y,21.0f), FRotator(0,0,0));
		GetWorld()->GetTimerManager().SetTimer(Pause, this, &URangedAttack::SpawnAttack, 0.75f, false);
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	owner.GetBlackboardComponent()->SetValueAsVector(FName("Spawn"), SpawnLoc);
	UE_LOG(LogTemp, Warning, TEXT("the blackboard key is set: %s"), *SpawnLoc.ToString());
	return EBTNodeResult::Succeeded;
}

void URangedAttack::SpawnAttack()
{
	FActorSpawnParameters SpawnerParams;
	SpawnerParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARangedOrb* Attack = GetWorld()->SpawnActor<ARangedOrb>(RB, SpawnLoc, FRotator(0, 0, 0), SpawnerParams);
}

