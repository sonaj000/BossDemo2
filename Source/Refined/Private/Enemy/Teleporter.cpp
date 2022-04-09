// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionProps/Teleporter.h"
#include "Character/MCharacter.h"
#include "Components/SphereComponent.h"
#include "Math/Vector.h"


// Sets default values
ATeleporter::ATeleporter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TeleportSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Teleporter"));
	RootComponent = TeleportSphere;

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(RootComponent);
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ATeleporter::OnOverLap);


}

void ATeleporter::OnOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		if (ToTeleport)
		{
			AMCharacter* Teleportee = Cast<AMCharacter>(OtherActor);
			UE_LOG(LogTemp,Warning,TEXT("This is cool"));

			Teleportee->SetActorLocation(ToTeleport->GetActorLocation());
		}
		
	}
}

// Called when the game starts or when spawned
void ATeleporter::BeginPlay()
{
	Super::BeginPlay();
	THere = ToTeleport->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Other Teleporter Location: %s"), *THere.ToString());
	
}

// Called every frame
void ATeleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

