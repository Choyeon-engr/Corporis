// Fill out your copyright notice in the Description page of Project Settings.


#include "CorporisHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CorporisChampion.h"

void UCorporisHUDWidget::BindChampionStat(ACorporisChampion* ChampionStat)
{
    CurrentChampionStat = ChampionStat;
    ChampionStat->OnHPChanged.AddUObject(this, &UCorporisHUDWidget::UpdateChampionStat);
    ChampionStat->BulletQuantityChanged.AddUObject(this, &UCorporisHUDWidget::UpdateChampionStat);
}

void UCorporisHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HealthBar")));
    BulletQuantity = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_BulletQuantity")));
}

void UCorporisHUDWidget::UpdateChampionStat()
{
    HealthBar->SetPercent(CurrentChampionStat->GetHPRatio());
    BulletQuantity->SetText(FText::FromString(FString::FromInt(CurrentChampionStat->GetBulletQuantity())));
}