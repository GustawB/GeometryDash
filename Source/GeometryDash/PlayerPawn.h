// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "GDCGameInstance.h"
#include "Obstacle.h"
#include "Bouncer.h"
#include "LevelActor.h"
#include "LevelEnd.h"
#include "BounceSphere.h"
#include "Portal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerPawn.generated.h"

UCLASS()
class GEOMETRYDASH_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int framesCountForLandingDetection{ 0 };

	int framesCountForSpeedChecking{ 0 };

	int framesCountForDisplayingLevelCompleted{ 0 };

	int framesCountForDoubleJumping{ 0 };

	int framesCountForPortalInteraction{ 10 };

	float b{ 0.0f };

	float c{ 0.0f };

	float d{ 0.0f };

	FVector PastPlayerLocation{};

	FVector PortalPlayerLocation{};

	bool isJumping{ false };

	bool DoubleJump{ false };

	bool bIsFlying{ false };

	bool bIsGoingUp{ false };

	UPROPERTY(VisibleDefaultsOnly)
		UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent* PlayerMesh;

	UPROPERTY(VisibleDefaultsOnly)
		USpringArmComponent* CameraSpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float MovementSpeed{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float JumpHeight{ -100.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float JumpBeginning{ -22.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float JumpChangePerFrame{ 0.8f };

	UPROPERTY(VisibleDefaultsOnly)
		FVector PlayerHeightWhenJumped{ 0.0f , 0.0f, 0.0f };

	UPROPERTY(VisibleDefaultsOnly)
		FRotator PlayerRotationWhenJumped{ 0.0f , 0.0f, 0.0f };

	UPROPERTY(VisibleDefaultsOnly)
		FVector StaticLocationOfPlayer {};

	UPROPERTY(VisibleDefaultsOnly)
		FRotator StaticRotationOfPlayer {};

	UPROPERTY(VisibleDefaultsOnly)
		bool bDidPlayerJump{true};

	UPROPERTY(VisibleDefaultsOnly)
		bool bDidPlayerLandedAfterJump{false};

	UPROPERTY(EditAnywhere)
		bool bShouldRestartLevel{ false };

	UFUNCTION(BlueprintCallable)
		void StartJump();

	UFUNCTION(BlueprintCallable)
		void StartFlying();

	UFUNCTION(BlueprintCallable)
		void StopFlying();

	UFUNCTION(BlueprintCallable)
		void CustomStartJump(float CustomJumpBeginning);

	UFUNCTION(BlueprintCallable)
		void StopJump();

	UFUNCTION(BlueprintCallable)
		void PauseGame();

	UFUNCTION(BLueprintCallable)
		void LevelPlayerRotation(FRotator& PlayerRotation );

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
