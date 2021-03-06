// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy_Base.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/AudioComponent.h"
#include "HealthComp.h"


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

	CloseAreaAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Close Ranged Component"));
	CloseAreaAudio->SetupAttachment(RootComponent);
	CloseAreaAudio->bAutoActivate = false;

	HealthBar = CreateDefaultSubobject<UHealthComp>(TEXT("Health Bar"));
	HealthBar->OnHealthChanged.AddDynamic(this, &AEnemy_Base::OnHealthChanged);
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

void AEnemy_Base::DelayedAudio()
{
	if (RangedAudio != nullptr)
	{
		RangedAudio->Play();
	}
}

void AEnemy_Base::RangeAttack()
{
	FString Name = "Boss_Ranged";
	if (RangeMontage)
	{
		PlayAnimMontage(RangeMontage, 1.5f, FName(*Name));
		FTimerHandle RA;
		GetWorld()->GetTimerManager().SetTimer(RA, this, &AEnemy_Base::DelayedAudio, 0.5f, false);
	}

}

void AEnemy_Base::CloseAreaAttack()
{
	FString Name = "CloseArea";
	if (TestMontage)
	{
		PlayAnimMontage(TestMontage, 1.5f, FName(*Name));
		if (CloseAreaAudio != nullptr)
		{
			CloseAreaAudio->Play();
		}
	}
}

// Called when the game starts or when spawned
void AEnemy_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy_Base::OnHealthChanged(UHealthComp* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	
	FString MontageSection = "Hit";
	if (TestMontage)
	{
		PlayAnimMontage(TestMontage, 1.5f, FName(*MontageSection));
		UE_LOG(LogTemp, Warning, TEXT("this is happening"));
	}
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

