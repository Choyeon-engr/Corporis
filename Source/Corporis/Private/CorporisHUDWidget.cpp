#include "CorporisHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CorporisChampion.h"

void UCorporisHUDWidget::BindChampionStat(ACorporisChampion* ChampionStat)
{
    CurrentChampionStat = ChampionStat;
    ChampionStat->OnHPChanged.AddUObject(this, &UCorporisHUDWidget::UpdateChampionStat);
    ChampionStat->OnBulletQuantityChanged.AddUObject(this, &UCorporisHUDWidget::UpdateChampionStat);
    ChampionStat->OnScoreChanged.AddUObject(this, &UCorporisHUDWidget::UpdateChampionStat);
    ChampionStat->OnDeathInfoChanged.AddUObject(this, &UCorporisHUDWidget::UpdateChampionStat);
}

void UCorporisHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HealthBar")));
    BulletQuantity = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_BulletQuantity")));
    CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_CurrentScore")));
    HighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_HighScore")));
    DeathInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_DeathInfo")));
}

void UCorporisHUDWidget::UpdateChampionStat()
{
    HealthBar->SetPercent(CurrentChampionStat->GetHPRatio());
    BulletQuantity->SetText(FText::FromString(FString::FromInt(CurrentChampionStat->GetBulletQuantity())));
    CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentChampionStat->GetCurrentScore())));
    HighScore->SetText(FText::FromString(FString::FromInt(CurrentChampionStat->GetHighScore())));
    DeathInfo->SetText(FText::FromString(FString(CurrentChampionStat->GetDeathInfo())));
}
