// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEnd.h"

// Sets default values
ALevelEnd::ALevelEnd()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}
	if (!LevelEnd)
	{
		LevelEnd = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LevelEnd"));
		LevelEnd->SetCollisionProfileName("LevelActor");
		RootComponent = LevelEnd;
	}

}

// Called when the game starts or when spawned
void ALevelEnd::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelEnd::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

