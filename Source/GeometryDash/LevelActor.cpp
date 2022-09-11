// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelActor.h"

// Sets default values
ALevelActor::ALevelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}
	if (!LevelActorMesh)
	{
		LevelActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LevelActorMesh"));
		LevelActorMesh->SetCollisionProfileName("LevelActor");
		RootComponent = LevelActorMesh;
	}

}

// Called when the game starts or when spawned
void ALevelActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

