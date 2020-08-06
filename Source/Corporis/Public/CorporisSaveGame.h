#pragma once

#include "Corporis.h"
#include "GameFramework/SaveGame.h"
#include "CorporisSaveGame.generated.h"

UCLASS()
class CORPORIS_API UCorporisSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
    UCorporisSaveGame();
    
    int32 GetHighScore() { return HighScore; }
    void SetHighScore(int32 HighestScore) { HighScore = HighestScore; }
    
private:
    UPROPERTY()
    int32 HighScore;
};
