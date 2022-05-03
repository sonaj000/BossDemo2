// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy_Base.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/AudioComponent.h"


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

	CloseAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Close Audio Component"));
	CloseAudio->SetupAttachment(RootComponent);
	CloseAudio->bAutoActivate = false;

	RangedAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Ranged Audio Component"));
	RangedAudio->SetupAttachment(RootComponent);
	RangedAudio->bAutoActivate = false;

}

void AEnemy_Base::CloseAttack()
{
	FString MontageSection = "Boss_Close";
	if (TestMontage)
	{
		PlayAnimMontage(TestMontage, 1.5f, FName(*MontageSection));
		UE_LOG(LogTemp, Warning, TEXT("this is happening"));
	}
	if (CloseAudio != nullptr)
	{
		CloseAudio->Play();
	}

}

void AEnemy_Base::RangeAttack()
{
	FString Name = "Boss_Ranged";
	if (RangeMontage)
	{
		PlayAnimMontage(RangeMontage, 1.5f, FName(*Name));
	}
	if (RangedAudio != nullptr)
	{
		RangedAudio->Play();
	}
}

void AEnemy_Base::CloseAreaAttack()
{
	FString Name = "CloseArea";
	if (TestMontage)
	{
		PlayAnimMontage(TestMontage, 1.5f, FName(*Name));
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

