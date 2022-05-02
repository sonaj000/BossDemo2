// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss_Controller.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Character/MCharacter.h"
#include "Enemy/Enemy_Base.h"

//put the ai boss here



ABoss_Controller::ABoss_Controller(const FObjectInitializer& ObjectInitializer)
{

	BTC = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehavioralComp"));
	BBC = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight_Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp")));

	SightConfig->SightRadius = BossSightRadius; //max distance boss starts percieving
	SightConfig->LoseSightRadius = BossLoseSight; //max distance in which boss starts to lose sight
	SightConfig->PeripheralVisionAngleDegrees = BossFieldofView;//how far the boss can see to the side
	SightConfig->SetMaxAge(BossSightAge); // how long the sight is remembered

	SightConfig->DetectionByAffiliation.bDetectEnemies = true; //determines if enemies or whatever trigger this sense
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation()); //if we utilize more than one sense 
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ABoss_Controller::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	bisCharacterDetected = false;
	DistanceToPlayer = 0.0;
}

void ABoss_Controller::BeginPlay()
{
	Super::BeginPlay();

}

void ABoss_Controller::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);
	
	AEnemy_Base* chr = Cast<AEnemy_Base>(inPawn);
	if (chr != nullptr && chr->TreeAsset != nullptr)
	{
		BBC->InitializeBlackboard(*chr->TreeAsset->BlackboardAsset);
		RunBehaviorTree(chr->TreeAsset);
		BTC->StartTree(*chr->TreeAsset);
	}
}

UBlackboardComponent* ABoss_Controller::get_blackboard() const
{
	return BBC;
}

void ABoss_Controller::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (size_t i{ 0 }; i < DetectedPawns.Num(); i++)
	{
		if (DetectedPawns[i]->IsA(AMCharacter::StaticClass()))
		{
			DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
			Holder = DetectedPawns[i];
			// two modes of attack based on distance to player.
		   //need to input the detected pawn and the distance to player. 
			bisCharacterDetected = true;
			get_blackboard()->SetValueAsBool(FName("bisCharacterDetected"), bisCharacterDetected);// need to set blackboard keys for each. 
		}

	}
}

