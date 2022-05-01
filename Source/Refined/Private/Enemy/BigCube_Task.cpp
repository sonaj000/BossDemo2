// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BigCube_Task.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy/Boss_Controller.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enemy/Enemy_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Controller.h"
#include "Enemy/BigBox.h"

UBigCube_Task::UBigCube_Task()
{
	NodeName = "Big Cube Throw";
}

EBTNodeResult::Type UBigCube_Task::ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory)
{
	ABoss_Controller* Boss = Cast<ABoss_Controller>(owner.GetAIOwner());
	AEnemy_Base* B = Cast<AEnemy_Base>(owner.GetAIOwner()->GetPawn());
	if (B)
	{
		B->CloseAttack();
		TArray<FVector>Spawns;
		FVector Current = B->GetActorLocation();
		Spawns.Add(Current + FVector(0, 0, 400.0f));
		FVector A = Current + FVector(0, 400, 400.0f);
		Spawns.Add(A);
		FVector E = Current + FVector(400, 0, 400.0f);
		Spawns.Add(E);
		FVector C = Current + FVector(400, 0, 0.0f);
		Spawns.Add(C);
		FVector D = Current + FVector(0, 400, 0.0f);
		Spawns.Add(D);

		FActorSpawnParameters SpawnerParams;
		SpawnerParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Rot = Boss->Holder->GetActorLocation() + Boss->Holder->GetActorForwardVector() * 5;
		FRotator Turn = UKismetMathLibrary::FindLookAtRotation(B->GetActorLocation(), Rot);
		B->SetActorRotation(Turn);

		for (FVector loc : Spawns)
		{
			ABigBox* Attack = GetWorld()->SpawnActor<ABigBox>(BB, loc, FRotator(45, 45, 45), SpawnerParams);
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}



