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
	//HitBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::WeaponTrace);
	//HitBox->OnComponentEndOverlap.AddDynamic(this, &AWeapon::EndTrace);
	
	WeaponSocketName = "WeaponSocket";

	bcanTrace = false;
}



void AWeapon::WeaponT()
{
	FVector curBase = MeshComp->GetSocketLocation(FName("handle")), curTip = MeshComp->GetSocketLocation(FName("tip"));
	const int sub = 32;
	float curLength = (curBase - curTip).Size();
	float prevLength = (prevBase - prevTip).Size();
	for (int i = 1; i < sub; i++)
	{
		FVector tmpBase = FMath::Lerp(curBase, prevBase, i / float(sub));
		FVector tmpTip = FMath::Lerp(curTip, prevTip, i / float(sub));
		FVector tmpOff = (tmpTip - tmpBase);
		tmpOff.Normalize();
		DrawDebugLine(GetWorld(), tmpBase, tmpBase + tmpOff * FMath::Lerp(curLength, prevLength, i / float(sub)), FColor::Red, true, 1 / 15.0f * 2);
	}
	prevBase = curBase;
	prevTip = curTip;
	DrawDebugLine(GetWorld(), curBase, curTip, FColor::Green, true, 1 / 15.0f * 2);

}

void AWeapon::WeaponTrace(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bcanTrace && OtherActor->IsA(AActor::StaticClass()))
	{
		UStaticMeshComponent* PushBack = Cast<UStaticMeshComponent>(OtherActor->GetRootComponent());
		if (PushBack != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("weapon overlap"));
			PushBack->AddImpulse(OtherActor->GetActorForwardVector() * PushBack->GetMass() * -1000.0f);
			//use launch character if we have to
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("is null"));
		}
	}
}

void AWeapon::EndTrace(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && bcanTrace)
	{
		bcanTrace = false;
		UE_LOG(LogTemp, Warning, TEXT("has been set false"));
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
	prevBase = FVector::ZeroVector;
	prevTip = FVector::ZeroVector;

}

