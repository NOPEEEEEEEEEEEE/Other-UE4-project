// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ShooterSaveGameObject.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UShooterSaveGameObject : public USaveGame
{
	GENERATED_BODY()
	
public:
	UShooterSaveGameObject();


    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString PlayerName;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString SaveSlotName;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        uint32 UserIndex;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        int32 AllTimeKillCount=0;


};
