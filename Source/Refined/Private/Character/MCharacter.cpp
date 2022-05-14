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
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "HealthComp.h"


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

	FreezeAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Freeze Audio"));
	FreezeAudio->SetupAttachment(RootComponent);
	FreezeAudio->bAutoActivate = false;

	HealthBar = CreateDefaultSubobject<UHealthComp>(TEXT("Health Bar"));
	HealthBar->OnHealthChanged.AddDynamic(this,&AMCharacter::OnHealthChanged);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMCharacter::DashOverLap);

	JumpHeight = 600.0f;
	bcanDash = true;
	bisDash = false;
	DashCD = 0.2f;
	DashR = 0.1f;
	DashDistance = 1500.0f;

	WalkSpeed = 750.0f;
	RunSpeed = 1100.0f;
	bCanFreeze = true;
}

void AMCharacter::BeginPlay()
{
	Super::BeginPlay();
	JumpCounter = 0;
	
	//idk how effective this is. 

}

void AMCharacter::Tick(float DeltaTime)
{
	Time.TickTimeline(DeltaTime);
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
	GetCharacterMovement()->BrakingFrictionFactor = 0.1f;
}

void AMCharacter::DashReset()
{
	bcanDash = true;
	bisDash = false;
}

void AMCharacter::DashOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bisDash && bCanFreeze)
	{
		bCanFreeze = false;
		UE_LOG(LogTemp, Warning, TEXT("stop time"));
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1);
		//ugameplaystatics::Getactors of class enemy and class ranged orb will slow down enemy and all projectiles
		this->CustomTimeDilation = 10.0f;
		FTimerHandle WindTime;
		GetWorld()->GetTimerManager().SetTimer(WindTime, this, &AMCharacter::TimeReset, 0.1f, false);
		FTimerHandle FreezeReset;
		GetWorld()->GetTimerManager().SetTimer(FreezeReset, this, &AMCharacter::FreezeReset, 1.0f, false);
		FreezeAudio->Play();
		//play sound and spawn decal 
	}

}

void AMCharacter::TimeReset()
{
	UE_LOG(LogTemp, Warning, TEXT("rewind"));
	this->CustomTimeDilation = 1.0f;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
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


	PlayerInputComponent->BindAxis("MoveForward", this, &AMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMCharacter::AddControllerPitchInput);

}


