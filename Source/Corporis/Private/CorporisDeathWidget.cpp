#include "CorporisDeathWidget.h"
#include "CorporisChampion.h"
#include "Components/TextBlock.h"

void UCorporisDeathWidget::BindChampionStat(ACorporisChampion* ChampionStat)
{
    CurrentChampionStat = ChampionStat;
    ChampionStat->OnDeathInfoChanged.AddUObject(this, &UCorporisDeathWidget::UpdateChampionStat);
}

void UCorporisDeathWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    DeathInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_DeathInfo")));
}

void UCorporisDeathWidget::UpdateChampionStat()
{
    DeathInfo->SetText(FText::FromString(FString(CurrentChampionStat->GetDeathInfo())));
}
