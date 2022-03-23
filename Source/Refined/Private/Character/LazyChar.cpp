// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LazyChar.h"

ALazyChar::ALazyChar()
{
	JumpHeight = 600.0f;

}

void ALazyChar::BeginPlay()
{
	Super::BeginPlay();
	JumpCounter = 0;
}


void ALazyChar::DoubleJump()
{
	binAir = true;
	UE_LOG(LogTemp, Warning, TEXT("jumping"));
	if (JumpCounter <= 2)
	{

		this->LaunchCharacter(FVector(0, 0.0f, JumpHeight), false, true);
		JumpCounter++;
	}
}

void ALazyChar::Landed(const FHitResult& Hit)
{
	JumpCounter = 0;
	binAir = false;
}

void ALazyChar::Dodge()
{
}

void ALazyChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Test", IE_Pressed, this, &ALazyChar::DoubleJump);
}


