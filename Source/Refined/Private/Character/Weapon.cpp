// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapon.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy/Enemy_Base.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	SetRootComponent(MeshComp);

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(RootComponent);
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::WeaponTrace);
	
	WeaponSocketName = "WeaponSocket";

	bcanTrace = false;
}



void AWeapon::WeaponTrace(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bcanTrace && OtherActor->IsA(AEnemy_Base::StaticClass()))
	{
		UStaticMeshComponent* PushBack = Cast<UStaticMeshComponent>(OtherActor->GetRootComponent());
		if (PushBack != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("is not null"));
			PushBack->AddImpulse(OtherActor->GetActorForwardVector() * PushBack->GetMass() * -1000.0f);
			//use launch character if we have to
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("is null"));
		}
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

