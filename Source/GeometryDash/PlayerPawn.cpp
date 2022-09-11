// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	if (!PlayerMesh)
	{
		PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>NewMesh(TEXT("'/Game/Blueprints/Cube_Shape.Cube_Shape'"));
		if (NewMesh.Succeeded())
		{
			PlayerMesh->SetStaticMesh(NewMesh.Object);
		}
		PlayerMesh->SetSimulatePhysics(true);
		PlayerMesh->OnComponentHit.AddDynamic(this, &APlayerPawn::OnHit);
		PlayerMesh->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlapBegin);
		PlayerMesh->SetCollisionProfileName("Player");
		PlayerMesh->SetNotifyRigidBodyCollision(true);
		RootComponent = PlayerMesh;
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

	MovementSpeed = 12.0f;
	PastPlayerLocation = { 0.0f, 0.0f, 0.0f };
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	StaticLocationOfPlayer = GetActorLocation();
	StaticRotationOfPlayer = GetActorRotation();
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, GetActorForwardVector().ToString());
	Super::Tick(DeltaTime);
	if(bDidPlayerJump)
		++framesCountForLandingDetection;
	++framesCountForSpeedChecking;
	++framesCountForDoubleJumping;
	if (framesCountForDoubleJumping > 10)
		DoubleJump = false;
	FVector PlayerLocation{ GetActorLocation() };
	PlayerLocation.Y += MovementSpeed;
	FRotator PlayerRotation{ GetActorRotation() };
	//JumpHeight is set by default to -100.0f. If player jumps, its value is changed, so the condition is true. Then, player follows y = -x^2+ 484
	//which results in a beautiful jump. PlayerHeightWhenJumped stores player's location when he presses jump button 
	if (JumpHeight > -100.0f && (!bIsFlying))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Calc"));
		float AddHeight{ -JumpHeight * JumpHeight + b };
		PlayerLocation.Z = PlayerHeightWhenJumped.Z + AddHeight;
		PlayerRotation.Roll += 1.5f;
		//something with frames, I'm lost
		JumpHeight += JumpChangePerFrame;
		if (bDidPlayerLandedAfterJump == true)
		{
			JumpHeight = -100.0f;
			bDidPlayerJump = false;
			bDidPlayerLandedAfterJump = false;
			PlayerRotation.Roll = PlayerRotationWhenJumped.Roll + 90.0f;
			LevelPlayerRotation(PlayerRotation);
			if (isJumping == true)
				StartJump();
		}
	}
	else if(bIsFlying)
	{
		if (!bIsGoingUp)
		{
			d = 0.0f;
			c += 10.0f;
			FVector TempPlayerLocation(GetActorLocation());
			TempPlayerLocation.Z -= 100.0f;
			SetActorLocation(TempPlayerLocation);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("LevelEnd"));
			c = 0.0f;
			d += 100.0f;
			FVector TempPlayerLocation(GetActorLocation());
			TempPlayerLocation.Z += 1000.0f;
			SetActorLocation(TempPlayerLocation);
		}
	}
	PlayerLocation.X = 2045.0f;
	PlayerRotation.Pitch = StaticRotationOfPlayer.Pitch;
	PlayerRotation.Yaw = StaticRotationOfPlayer.Yaw;
	SetActorLocation(PlayerLocation, false);
	SetActorRotation(PlayerRotation);

	//I had some issues with the pawn slowing down when landing at the oedge of the platform, so code below checks,
	//how much distance pawn covers in one frame, and if that value isn't equal to its initial speed,
	//we add additional value to player's location, and that value represents the difference between initial and current speed
	double difference{ GetActorLocation().Y - PastPlayerLocation.Y };
	//framesCountForSpeedChecking guarantees that there won't be any strange errors at the beginning of the game
	if ((difference < MovementSpeed-0.1f || difference > MovementSpeed+0.1f)&&framesCountForSpeedChecking>5)
	{
		double speedChange{ MovementSpeed - difference };
		PlayerLocation.Y += speedChange;
		SetActorLocation(PlayerLocation);
	}
	PastPlayerLocation = GetActorLocation();
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerPawn::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerPawn::StopJump);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerPawn::PauseGame);
	PlayerInputComponent->BindAction("Flying", IE_Pressed, this, &APlayerPawn::StartFlying);
	PlayerInputComponent->BindAction("Flying", IE_Released, this, &APlayerPawn::StopFlying);
}

void APlayerPawn::StartJump()
{
	isJumping = true;
	//this line prevents player from double jumping into oblivion, by locking PlayerHeightWhenJumped during current jump, so it won't be ovverwritten
	if (JumpHeight < -50.0f || DoubleJump)
	{
		b = 484.0f;
		//values used to determine rotation and height during a jump
		PlayerHeightWhenJumped = GetActorLocation();
		PlayerRotationWhenJumped = GetActorRotation();
		JumpHeight = JumpBeginning;
		bDidPlayerJump = true;
		//updated every frame, if player jumped it is responsible for delaying the moment of setting bDidPlayerLandedAfterJump (using it without the delay caused engine to detect collision before jump, so the jump ended to quickly)
		framesCountForLandingDetection = 0;
		JumpChangePerFrame = 0.8f;
	}
}

void APlayerPawn::StartFlying()
{
	if (bIsFlying)
	{
		bIsGoingUp = true;
	}
}

void APlayerPawn::StopFlying()
{
	if (bIsFlying)
	{
		bIsGoingUp = false;
	}
}

void APlayerPawn::CustomStartJump(float CustomJumpBeginning)
{
	isJumping = true;
	//this line prevents player from double jumping into oblivion, by locking PlayerHeightWhenJumped during current jump, so it won't be ovverwritten
	if (JumpHeight < -50.0f)
	{
		b = 900.0f;
		PlayerHeightWhenJumped = GetActorLocation();
		PlayerRotationWhenJumped = GetActorRotation();
		JumpHeight = CustomJumpBeginning;
		bDidPlayerJump = true;
		//updated every frame, if player jumped it is responsible for delaying the moment of setting bDidPlayerLandedAfterJump (using it without the delay caused engine to detect collision before jump, so the jump ended to quickly)
		framesCountForLandingDetection = 0;
		JumpChangePerFrame = 0.6f;
	}
	StopJump();
}

void APlayerPawn::StopJump()
{
	isJumping = false;
}

void APlayerPawn::PauseGame()
{
	UGDCGameInstance* GameInstance = Cast<UGDCGameInstance>(GetGameInstance());
	if (GameInstance != nullptr)
	{
		GameInstance->bCanShowPauseMenu = true;
	}
}

//small correction for longer and shorter jumps, so when for e.g. player rotated for 86 or 94 degrees, we auomatically correct that to 90 degrees
//also, sometimes player could bounce from strange rotation, so in order to compensate that, 
//a function below cuts the part of the roll that can't be divided by 90, and rounds that value to 0 or 90
void APlayerPawn::LevelPlayerRotation(FRotator& PlayerRotation)
{
	double roll{ PlayerRotation.Roll };
	while (roll > 0)
		roll -= 90.0f;
	roll += 90.0f;
	double difference{ PlayerRotation.Roll - roll };
	if (roll != 0.0f && roll != 90.0f)
	{
		if (90.0f - roll < roll)
			roll = 90.0f;
		else
			roll = 0.0f;
	}
	PlayerRotation.Roll = difference + roll;
}

void APlayerPawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Casting Other actor to check wheter it's obstacle. If cast succeeds (OtherActor is an obstacle), then Obstacle object isn't nullptr
	AObstacle* Obstacle = Cast<AObstacle>(OtherActor);
	ABouncer* Bouncer = Cast<ABouncer>(OtherActor);
	ALevelActor* LevelActor = Cast<ALevelActor>(OtherActor);
	ALevelEnd* LevelEnd = Cast<ALevelEnd>(OtherActor);
	if (Obstacle != nullptr)
	{
		UGameplayStatics::OpenLevel(this, FName(GetWorld()->GetName()), false);
		bShouldRestartLevel = true;
	}
	else if (Bouncer != nullptr)
	{
		if (isJumping == false)
			CustomStartJump(-30.0f);
	}
	else if (LevelActor != nullptr)
	{
		double LevelActorHeight{ LevelActor->GetActorLocation().Z };
		FVector TempPlayerLocation{ GetActorLocation() };
		FRotator TempPlayerRotation{ GetActorRotation() };
		TempPlayerLocation.Z = LevelActorHeight + 70.0f;
		TempPlayerRotation.Roll = PlayerRotationWhenJumped.Roll + 90.0f;
		LevelPlayerRotation(TempPlayerRotation);
		SetActorLocation(TempPlayerLocation);
		//if(LevelActor->bShouldUpdateRotation)
		SetActorRotation(TempPlayerRotation);
	}
	else if (LevelEnd != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("LevelEnd"));
		UGDCGameInstance* GameInstance = Cast<UGDCGameInstance>(GetGameInstance());
		//before that, level completed widget would be displayed twice, because player had a contact with the LevelEnd for 4 frames. Now he has contact for 6 or more seconds,
		//but the problem is no longer existent
		++framesCountForDisplayingLevelCompleted;
		if (GameInstance != nullptr && framesCountForDisplayingLevelCompleted == 4)
			GameInstance->bCanShowLevelCompleted = true;
	}
	if (OtherActor != this)
	{
		//if player jumped and we are sure that bool will be set to true 4 frames after the landing, inform compiler (or something) that player has landed
		if (bDidPlayerJump && framesCountForLandingDetection > 4)
		{
			bDidPlayerLandedAfterJump = true;
			framesCountForLandingDetection = 0;
		}
	}
}

void APlayerPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABounceSphere* BounceSphere = Cast<ABounceSphere>(OtherActor);
	APortal* Portal = Cast<APortal>(OtherActor);
	if (BounceSphere != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("BounceSphere"));
		//DoubleJump determines, whether player collided with BounceSPhere and can jump once more.
		//framesCountForDoubleJump determines the size of a "window" for player to jump. Modified in Tick function.
		DoubleJump = true;
		framesCountForDoubleJumping = 0;
		double LevelActorHeight{ BounceSphere->GetActorLocation().Z };
		FVector TempPlayerLocation{ GetActorLocation() };
		FRotator TempPlayerRotation{ GetActorRotation() };
		TempPlayerLocation.Z = LevelActorHeight + 70.0f;
		TempPlayerRotation.Roll = PlayerRotationWhenJumped.Roll + 90.0f;
		LevelPlayerRotation(TempPlayerRotation);
		SetActorLocation(TempPlayerLocation);
	}
	//Portal not working
	else if (Portal != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Portal"));
		/*PortalPlayerLocation = GetActorLocation();
		framesCountForPortalInteraction = 0;
		bDidPlayerLandedAfterJump = false;
		JumpHeight = -100.0f;*/
		bIsFlying = true;
		JumpHeight = -200.0f;
		bDidPlayerJump = false;
		PlayerMesh->SetEnableGravity(false);
	}
}

