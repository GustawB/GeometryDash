// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "TestPlayerPawn.generated.h"

UCLASS()
class GEOMETRYDASH_API ATestPlayerPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleDefaultsOnly)
		UCharacterMovementComponent* PlayerMovementComponent;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PlayerMeshComponent;

	UPROPERTY(VisibleDefaultsOnly)
		USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		UCameraComponent* PlayerCamera;

	//UPROPERTY(VisibleDefaultsOnly)
		//UCapsuleComponent* PlayerCapsuleComponent;
};
