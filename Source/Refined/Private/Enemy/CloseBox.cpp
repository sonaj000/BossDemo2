// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CloseBox.h"

ACloseBox::ACloseBox()
{
	bCanMove = false;
	Delay = 1.0f;
}

void ACloseBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCanMove)
	{
		FVector Curr = GetActorLocation();
		FVector New = Curr + GetActorForwardVector() * 2500.0f * DeltaTime;
		this->SetActorLocation(New);
	}
}

void ACloseBox::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle Change;
	GetWorld()->GetTimerManager().SetTimer(Change, this, &ACloseBox::Move, Delay, false);
}

void ACloseBox::Move()
{
	bCanMove = true;
}
