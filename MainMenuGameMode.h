// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AMainMenuGameMode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintPure)
		int32 GetTotalKills();

private:
	void Load();
	int32 TotalKills;


};
