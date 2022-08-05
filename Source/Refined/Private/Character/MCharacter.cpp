// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "HealthComp.h"
#include "Enemy/RangedOrb.h"
#include "Enemy/Enemy_Base.h"
#include "Character/Weapon.h"
#include "GameFramework/DamageType.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
AMCharacter::AMCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComp->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	WeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponComp->SetupAttachment(RootComponent);

	FreezeAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Freeze Audio"));
	FreezeAudio->SetupAttachment(RootComponent);
	FreezeAudio->bAutoActivate = false;

	HealthBar = CreateDefaultSubobject<UHealthComp>(TEXT("Health Bar"));
	HealthBar->OnHealthChanged.AddDynamic(this,&AMCharacter::OnHealthChanged);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMCharacter::DashOverLap);

	JumpHeight = 5000.0f;
	bcanDash = true;
	bisDash = false;
	DashCD = 0.2f;
	DashR = 0.1f;
	DashDistance = 1500.0f;

	WeaponAttachSocketName = "Test";

	WalkSpeed = 750.0f;
	RunSpeed = 2000.0f;
	bCanFreeze = true;
}

void AMCharacter::BeginPlay()
{
	Super::BeginPlay();
	JumpCounter = 0;
	ComboCounter = 0;
	bCanAttack = true;
	bCanFinish = true;
	bCanSweep = true;
	bCanLunge = true;
	OrgRot = this->GetActorRotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(SpawnWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	UE_LOG(LogTemp, Warning, TEXT("weapon spawned"));
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		UE_LOG(LogTemp, Warning, TEXT("weapon attached")); //has been successfully done
	}

}

void AMCharacter::Tick(float DeltaTime)
{
}

void AMCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMCharacter::Attack()
{
	//note that this is only for ground and not in air
	TArray<FHitResult>EnemiesHit;
	TArray<FHitResult>EnemiesDetected;
	FVector Start = GetActorForwardVector();
	FVector End = GetActorLocation() + GetActorForwardVector() * 100;
	FVector Loc = this->GetActorLocation() + FVector(0, 0, 50);
	FCollisionQueryParams QueryParams;

	QueryParams.AddIgnoredActor(this); // ignore the character
	QueryParams.AddIgnoredActor(CurrentWeapon);
	QueryParams.bTraceComplex = true;
	
	FCollisionObjectQueryParams Objectparams;
	
	Objectparams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	Objectparams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	const FVector BoxCoord = FVector(100, 100, 100);
	FCollisionShape MyBox = FCollisionShape::MakeBox(BoxCoord);

	const FVector DCoord = FVector(350, 350, 100);
	FCollisionShape DetectBox = FCollisionShape::MakeBox(DCoord);

	//set character movement to zero in order to stop movement while attacking
	GetCharacterMovement()->MaxWalkSpeed = 0;
	FTimerHandle Walk;
	GetWorld()->GetTimerManager().SetTimer(Walk, this, &AMCharacter::MovementReset, 0.2f, false);
	GetWorld()->GetTimerManager().SetTimer(AR, this, &AMCharacter::AttackReset, 1.0f, false);//delay the activation of timer reset by a second
	FString MontageSection = FString::FromInt(ComboCounter);
	FString DownAttack = "down";
	bCanAttack = GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage);
	if (binAir && !bCanAttack && AttackMontage)
	{
		LaunchCharacter(GetActorUpVector() * -1000, true, true);
		PlayAnimMontage(AttackMontage, 2.5f, FName(*DownAttack));
	}

	else if (AttackMontage && !bCanAttack && !binAir)
	{
		bool isHit = GetWorld()->SweepMultiByObjectType(EnemiesHit, End, End, FQuat::Identity, Objectparams, MyBox,QueryParams);
		DrawDebugBox(GetWorld(), End, BoxCoord, FColor::Purple, false, -1, 0, 3);

		bool isDetect = GetWorld()->SweepMultiByObjectType(EnemiesDetected, Loc, Loc, FQuat::Identity, Objectparams, DetectBox, QueryParams);
		DrawDebugBox(GetWorld(), Loc, DCoord, FColor::Red, false, -1, 0, 3);
		//look and rotate towards first enemy in the hit
		if (EnemiesDetected.Num() > 0)
		{
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), EnemiesDetected[0].GetActor()->GetActorLocation());
			FRotator fixed = FRotator(OrgRot.Pitch, Rotation.Yaw, Rotation.Roll);
			this->SetActorRotation(fixed);
		}
		//damage apply
		UE_LOG(LogTemp,Warning, TEXT("number of enemies hit : %d"), EnemiesHit.Num())
		if (isHit)
		{
			for (auto& Hit : EnemiesHit)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, FString::Printf(TEXT("Hit Result : %s "), *Hit.Actor->GetName()));
					AActor* HitTarget = Hit.GetActor();
					UGameplayStatics::ApplyDamage(HitTarget, 5.0f, CurrentWeapon->GetInstigatorController(), this, NAD);
					UStaticMeshComponent* PushBack = Cast<UStaticMeshComponent>(HitTarget->GetRootComponent());
					if (PushBack != nullptr)
					{
						UE_LOG(LogTemp, Warning, TEXT("is not null"));
						PushBack->AddImpulse(HitTarget->GetActorUpVector() * PushBack->GetMass() * 1000.0f);
						//use launch character if we have to
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("is null"));
					}

				}
			}
		}

		if (ComboCounter == 0)
		{
			PlayAnimMontage(AttackMontage, 3.0f, FName(*MontageSection));
			UE_LOG(LogTemp, Warning, TEXT("montage1"));
			ComboCounter += 1;
		}
		else if (ComboCounter == 1)
		{
			PlayAnimMontage(AttackMontage, 3.0f, FName(*MontageSection));
			UE_LOG(LogTemp, Warning, TEXT("montage2"));
			ComboCounter += 1;
		}
		else if (ComboCounter == 2)
		{
			PlayAnimMontage(AttackMontage, 3.0f, FName(*MontageSection));
			UE_LOG(LogTemp, Warning, TEXT("montage2"));
			ComboCounter = 0;
		}

	}
	UE_LOG(LogTemp, Warning, TEXT("attacking"));

}


void AMCharacter::AttackReset()
{
	APlayerController* MController = Cast<APlayerController>(GetController());
	if ((GetWorldTimerManager().GetTimerElapsed(AR) < 1.1f) && MController->IsInputKeyDown(FKey(EKeys::LeftMouseButton)))
	{
		GetWorldTimerManager().ClearTimer(AR);
	}
	else
	{
		ComboCounter = 0;
	}
}

void AMCharacter::MovementReset()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AMCharacter::SkillLunge()
{
	FString MontageSection = "Lunge";

	TArray<FHitResult>EnemiesHit;
	const FVector BCoord = FVector(100, 100, 100);
	FCollisionShape HBox = FCollisionShape::MakeBox(BCoord);
	FVector Start = GetActorForwardVector();
	FVector End = GetActorLocation() + GetActorForwardVector() * 100;
	FVector Loc = this->GetActorLocation() + FVector(0, 0, 50);
	FCollisionQueryParams QueryParams;

	QueryParams.AddIgnoredActor(this); // ignore the character
	QueryParams.AddIgnoredActor(CurrentWeapon);
	QueryParams.bTraceComplex = true;

	FCollisionObjectQueryParams Objectparams;

	Objectparams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	Objectparams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	if (SkillMontage && bCanLunge)
	{
		PlayAnimMontage(SkillMontage, 2.0f, FName(*MontageSection));
		bool isHit = GetWorld()->SweepMultiByObjectType(EnemiesHit, End, End, FQuat::Identity, Objectparams, HBox, QueryParams);
		DrawDebugBox(GetWorld(), End, BCoord, FColor::Blue, true, -1, 0, 3);
		//damage apply
		if (isHit)
		{
			for (auto& Hit : EnemiesHit)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, FString::Printf(TEXT("Hit Result : %s "), *Hit.Actor->GetName()));
					AActor* HitTarget = Hit.GetActor();
					UGameplayStatics::ApplyDamage(HitTarget, 5.0f, CurrentWeapon->GetInstigatorController(), this, NAD);
					UStaticMeshComponent* PushBack = Cast<UStaticMeshComponent>(HitTarget->GetRootComponent());
					if (PushBack != nullptr)
					{
						UE_LOG(LogTemp, Warning, TEXT("is not null"));
						PushBack->AddImpulse(HitTarget->GetActorUpVector() * PushBack->GetMass() * 1000.0f);
						//use launch character if we have to
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("is null"));
					}

				}
			}
		}
		//stuff
		UNiagaraComponent* ConeEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Cone, this->GetActorLocation() - FVector(0,0,75), FRotator(-180, 180, -180.0f));
		UE_LOG(LogTemp, Warning, TEXT("skill montage"));
		bCanLunge = false;
		int te = 1;
		UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (bCanLunge ? TEXT("true") : TEXT("false")));
		FTimerHandle LungeDelay;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AMCharacter::SkillReset, te);
		GetWorld()->GetTimerManager().SetTimer(LungeDelay, Delegate, 2.0f, false);
	}
}

void AMCharacter::ChargeSweep()
{
	FString MontageSection = "ChargeSwing";
	if (SkillMontage && bCanSweep)
	{
		CurrentWeapon->WeaponT();
		PlayAnimMontage(SkillMontage, 2.0f, FName(*MontageSection));
		UNiagaraComponent* ConeEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SweepEffect, this->GetActorLocation(), FRotator(90, 90, 0.0f));
		UE_LOG(LogTemp, Warning, TEXT("skill montage"));
		bCanSweep = false;
		int te = 2;
		UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (bCanSweep ? TEXT("true") : TEXT("false")));
		FTimerHandle SweepDelay;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AMCharacter::SkillReset, te);
		GetWorld()->GetTimerManager().SetTimer(SweepDelay, Delegate, 2.0f, false);
	}
}

void AMCharacter::Finisher()
{
	FString MontageSection = "Finisher";
	if (SkillMontage && bCanFinish)
	{
		CurrentWeapon->WeaponT();
		PlayAnimMontage(SkillMontage, 1.0f, FName(*MontageSection));
		UNiagaraComponent* Spin = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpinEffect, this->GetActorLocation(), FRotator(-180, -180, -180.0f), FVector(2.0, 2.0, 5.0f));
		//weapon - boolean true for linetrace or onhitcomponent
		UE_LOG(LogTemp, Warning, TEXT("skill montage"));
		bCanFinish = false;
		int te = 3;
		UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (bCanSweep ? TEXT("true") : TEXT("false")));

		FTimerHandle FinishDelay;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AMCharacter::SkillReset, te);
		GetWorld()->GetTimerManager().SetTimer(FinishDelay, Delegate, 2.0f, false);
	}
}


void AMCharacter::SkillReset(int skillnum)
{
	switch (skillnum)
	{
		case 1:
			bCanLunge = true;
			UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (bCanLunge ? TEXT("true") : TEXT("false")));
			break;
		case 2:
			bCanSweep = true;
			UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (bCanSweep ? TEXT("true") : TEXT("false")));
			break;
		case 3:
			bCanFinish = true;
			UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (bCanFinish ? TEXT("true") : TEXT("false")));
			break;
	}




}

void AMCharacter::RunStart()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AMCharacter::RunEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}


void AMCharacter::DoubleJump()
{
	binAir = true;
	UE_LOG(LogTemp, Warning, TEXT("jumping"));
	if (JumpCounter < 2)
	{

		AMCharacter::LaunchCharacter(FVector(0, 0.0f, JumpHeight), false, true);
		JumpCounter++;
	}
}

void AMCharacter::Landed(const FHitResult& Hit)
{
	JumpCounter = 0;
	binAir = false;
}

void AMCharacter::Dodge()
{
	///stop all montages playing
	GetMesh()->GetAnimInstance()->Montage_Stop(NULL); //dash is priority montage will overtake all other inputs
	FTimerHandle Stop;
	if (bcanDash)
	{
		GetCharacterMovement()->BrakingFrictionFactor = 0.1f;//prevents friction slowing down during dash

		//instead of get forward vector we can use get last input vector. 
		const FVector Testing = GetLastMovementInputVector().GetSafeNormal();
		const FVector Try = this->GetActorRotation().Vector();
		const FVector Convert = FVector(Try.X, Try.Y, -0.1f);

		//clamp z 
		LaunchCharacter(Convert * DashDistance, true, true);

		bcanDash = false;
		bisDash = true;
		GetWorldTimerManager().SetTimer(Stop, this, &AMCharacter::DashStop, DashCD, false);//no loop timer
	}
}

void AMCharacter::DashStop()
{
	FTimerHandle DashS;
	GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(DashS, this, &AMCharacter::DashReset, DashR, false);
	GetCharacterMovement()->BrakingFrictionFactor = 8.0f;
}

void AMCharacter::DashReset()
{
	bcanDash = true;
	bisDash = false;
}

void AMCharacter::DashOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bisDash && bCanFreeze && !OtherActor->IsA(AWeapon::StaticClass()))
	{
		bCanFreeze = false;
		UE_LOG(LogTemp, Warning, TEXT("stop time"));
		UE_LOG(LogTemp, Warning, TEXT("actor hit is : %s"), *OtherActor->GetName());

		FString MontageSection = "DOverlap";
		if (AttackMontage)
		{
			PlayAnimMontage(AttackMontage, 3.0f, FName(*MontageSection));
		}

		UNiagaraComponent* Overlap = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DashEffect, this->GetActorLocation(),FRotator::ZeroRotator, FVector(2.0, 2.0, 2.0f));
		//ugameplaystatics::Getactors of class enemy and class ranged orb will slow down enemy and all projectiles
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy_Base::StaticClass(),TimeStore);
		TimeStore.Add(OtherActor);
		if (TimeStore.Num() > 0)
		{
			for (AActor* thing : TimeStore)
			{
				thing->CustomTimeDilation = 0.0f;
			}
		}
		FTimerHandle WindTime;
		GetWorld()->GetTimerManager().SetTimer(WindTime, this, &AMCharacter::TimeReset, 0.7f, false);
		FTimerHandle FreezeReset;
		GetWorld()->GetTimerManager().SetTimer(FreezeReset, this, &AMCharacter::FreezeReset, 0.7f, false);
		FreezeAudio->Play();
		//play sound and spawn decal 
	}

}

void AMCharacter::TimeReset()
{
	UE_LOG(LogTemp, Warning, TEXT("rewind"));
	for (AActor* thing : TimeStore)
	{
		thing->CustomTimeDilation = 1.0f;
	}
	TimeStore.Empty();
	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void AMCharacter::FreezeReset()
{
	bCanFreeze = true;
}

void AMCharacter::OnHealthChanged(UHealthComp* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f)
	{
		GetMovementComponent()->StopMovementImmediately();
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//DetachFromControllerPendingDestroy();

		//SetLifeSpan(5.0f);
		//UGameplayStatics::OpenLevel(this, FName("Menu_Map"), false);
		//change this to death scene later

	}
}

void AMCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("DoubleJump", IE_Pressed, this, &AMCharacter::DoubleJump);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AMCharacter::Dodge);
	PlayerInputComponent->BindAction("RunStart", IE_Pressed, this, &AMCharacter::RunStart);
	PlayerInputComponent->BindAction("RunEnd", IE_Released, this, &AMCharacter::RunEnd);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMCharacter::Attack);
	PlayerInputComponent->BindAction("Skill1", IE_Pressed, this, &AMCharacter::SkillLunge);
	PlayerInputComponent->BindAction("Skill2", IE_Pressed, this, &AMCharacter::ChargeSweep);
	PlayerInputComponent->BindAction("Skill3", IE_Pressed, this, &AMCharacter::Finisher);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMCharacter::AddControllerPitchInput);

}


