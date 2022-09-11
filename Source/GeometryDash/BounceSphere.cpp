// Fill out your copyright notice in the Description page of Project Settings.


#include "BounceSphere.h"

// Sets default values
ABounceSphere::ABounceSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if(!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}
	if (!BounceSphereMeshComponent)
	{
		BounceSphereMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BounceSphereMeshComponent"));
		BounceSphereMeshComponent->SetCollisionProfileName("BounceSphere");
		BounceSphereMeshComponent->SetVisibility(false);
		RootComponent = BounceSphereMeshComponent;
	}
	if (!ParticleComponent)
	{
		ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
		ParticleComponent->SetupAttachment(BounceSphereMeshComponent);
	}

}

// Called when the game starts or when spawned
void ABounceSphere::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABounceSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

