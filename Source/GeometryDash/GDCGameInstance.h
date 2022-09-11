// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GDCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GEOMETRYDASH_API UGDCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UGDCGameInstance(const FObjectInitializer& FObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanShowMainMenu{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanShowLevelCompleted{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanShowPauseMenu{};
	
};
