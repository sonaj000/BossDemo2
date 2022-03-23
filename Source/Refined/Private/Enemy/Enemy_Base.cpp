// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy_Base.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

// Sets default values
AEnemy_Base::AEnemy_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>TestMontageObject(TEXT("AnimMontage'/Game/Enemy/Anim/Boss_Close.Boss_Close'"));
	if (TestMontageObject.Succeeded())
	{
		TestMontage = TestMontageObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Montage Succeeded"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>RangeMontageObject(TEXT("AnimMontage'/Game/Enemy/Anim/Boss_Ranged.Boss_Ranged'"));
	if (RangeMontageObject.Succeeded())
	{
		RangeMontage = RangeMontageObject.Object;
	}

}

void AEnemy_Base::CloseAttack()
{
	FString MontageSection = "Boss_Close";
	if (TestMontage)
	{
		PlayAnimMontage(TestMontage, 1.5f, FName(*MontageSection));
		UE_LOG(LogTemp, Warning, TEXT("this is happening"));
	}

}

void AEnemy_Base::RangeAttack()
{
	FString Name = "Boss_Ranged";
	if (RangeMontage)
	{
		PlayAnimMontage(RangeMontage, 1.5f, FName(*Name));
	}
}

// Called when the game starts or when spawned
void AEnemy_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

