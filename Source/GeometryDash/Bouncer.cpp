// Fill out your copyright notice in the Description page of Project Settings.


#include "Bouncer.h"

// Sets default values
ABouncer::ABouncer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}
	if (!BouncerMeshComponent)
	{
		BouncerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BouncerMeshComponent"));
		BouncerMeshComponent->SetCollisionProfileName("Obstacle");
		RootComponent = BouncerMeshComponent;
	}
}

// Called when the game starts or when spawned
void ABouncer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABouncer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

