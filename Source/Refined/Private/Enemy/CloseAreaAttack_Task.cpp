// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CloseAreaAttack_Task.h"
#include "Enemy/Boss_Controller.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enemy/Enemy_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy/RangedOrb.h"


EBTNodeResult::Type UCloseAreaAttack_Task::ExecuteTask(UBehaviorTreeComponent& owner, uint8* node_memory)
{

	ABoss_Controller* Boss = Cast<ABoss_Controller>(owner.GetAIOwner());
	T = Cast<AEnemy_Base>(owner.GetAIOwner()->GetPawn());
	if (T)
	{
		TArray<FVector>Spawns;
		FVector Current = T->GetActorLocation();
		T->CloseAreaAttack();
		Spawns.Add(Current + T->GetActorForwardVector() * 300 + FVector(0, 0, -10.0f));
		FVector A = Current + T->GetActorForwardVector() * -300 + FVector(0, 0, -10.0f);
		Spawns.Add(A);
		FVector B = Current + T->GetActorRightVector() * 300 + FVector(0, 0, -10.0f);
		Spawns.Add(B);
		FVector C = Current + FVector(300, 0, 0.0f);
		//Spawns.Add(C);
		FVector D = Current + FVector(0, 300, 0.0f);
		//Spawns.Add(D);
		FVector E = Current + T->GetActorRightVector() * -300 + FVector(0, 0, -10.0f);
		Spawns.Add(E);

		for (FVector loc : Spawns)
		{
			FActorSpawnParameters SpawnerParams;
			SpawnerParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ARangedOrb* Exp = GetWorld()->SpawnActor<ARangedOrb>(Explode, loc, FRotator(45, 45, 45), SpawnerParams);
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	//animation done in boss.
	return EBTNodeResult::Succeeded;
}
