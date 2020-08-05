// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Corporis.h"
#include "GameFramework/SaveGame.h"
#include "CorporisSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CORPORIS_API UCorporisSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
    UCorporisSaveGame();
    
    UPROPERTY()
    int32 HighScore;
};
