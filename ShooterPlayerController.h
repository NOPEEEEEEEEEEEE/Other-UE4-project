// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	
	virtual void BeginPlay() override;

public:

	virtual void GameHasEnded(class AActor *EndGameFocus=nullptr, bool bIsWinner = false)override;

	UFUNCTION(BlueprintCallable)
	void CallGameHasEnded();

	void SetActualGameMode( bool MainMenu,bool KillEmAll,bool Survive);
	UFUNCTION(BlueprintPure)
	bool GetbMainMenu();
	UFUNCTION(BlueprintPure)
	bool GetbKillEmAll();
	UFUNCTION(BlueprintPure)
	bool GetbSurvive();
	UFUNCTION(BlueprintPure)
		int32 GetKillCount();
	UFUNCTION()
	void IncreaseKillCount();

	void OpenGameMenu();

private:
    UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget>HUDClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget>WinScreenClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget>LoseScreenClass;
	UPROPERTY(EditAnywhere)
		float RestartDelay = 5;
	FTimerHandle RestartTimer;

    UPROPERTY()
        int32 KillCount=0;


	UPROPERTY(EditAnywhere)
	UUserWidget* HUD;

	bool bMainMenu; 
	bool bKillEmAll; 
	bool bSurvive;


	void Save();
	void Load();
	int32 TotalKillsEver;
	

};
