#pragma once

#include "Corporis.h"
#include "Blueprint/UserWidget.h"
#include "CorporisHUDWidget.generated.h"

UCLASS()
class CORPORIS_API UCorporisHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void BindChampionStat(class ACorporisChampion* ChampionStat);
    
protected:
    virtual void NativeConstruct() override;
    
    void UpdateChampionStat();
    
private:
    UPROPERTY()
    class ACorporisChampion* CurrentChampionStat;
    
    UPROPERTY()
    class UProgressBar* HealthBar;
    
    UPROPERTY()
    class UTextBlock* BulletQuantity;
    
    UPROPERTY()
    class UTextBlock* CurrentScore;
    
    UPROPERTY()
    class UTextBlock* HighScore;
    
    UPROPERTY()
    class UTextBlock* KillInfo;
};
