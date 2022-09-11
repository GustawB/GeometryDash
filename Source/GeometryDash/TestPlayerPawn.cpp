// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayerPawn.h"

// Sets default values
ATestPlayerPawn::ATestPlayerPawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}
	if (!PlayerMeshComponent)
	{
		PlayerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>PlayerMesh(TEXT("'/Game/Blueprints/Cube_Shape.Cube_Shape'"));
		if (PlayerMesh.Succeeded())
			PlayerMeshComponent->SetStaticMesh(PlayerMesh.Object);
		RootComponent = PlayerMeshComponent;
		GetMovementComponent()->SetUpdatedComponent(PlayerMeshComponent);
	}
	if (!CameraSpringArm)
	{
		CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
		CameraSpringArm->SetupAttachment(RootComponent);
		CameraSpringArm->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when player does
		CameraSpringArm->TargetArmLength = 1500.f;
		CameraSpringArm->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		CameraSpringArm->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	}
	if (!PlayerCamera)
	{
		PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
		PlayerCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
		PlayerCamera->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm
	}
	//if (!PlayerMovementComponent)
	//{
		//PlayerMovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("PlayerMovementComponent"));
		//PlayerMovementComponent->SetUpdatedComponent(RootComponent);
	//}
	//if (!PlayerCapsuleComponent)
	//{
		//PlayerCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PlayerCapsuleComponent"));
		//PlayerCapsuleComponent->SetupAttachment(RootComponent);
	//}

}

// Called when the game starts or when spawned
void ATestPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetMovementComponent()->AddInputVector(FVector{ 0.0f, 100.0f, 0.0f });
}

// Called to bind functionality to input
void ATestPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATestPlayerPawn::Jump);

}

