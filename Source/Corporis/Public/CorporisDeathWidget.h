#pragma once

#include "Corporis.h"
#include "Blueprint/UserWidget.h"
#include "CorporisDeathWidget.generated.h"

UCLASS()
class CORPORIS_API UCorporisDeathWidget : public UUserWidget
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
    class UTextBlock* DeathInfo;
};
