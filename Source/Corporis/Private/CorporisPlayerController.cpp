// Fill out your copyright notice in the Description page of Project Settings.


#include "CorporisPlayerController.h"

ACorporisPlayerController::ACorporisPlayerController()
{
    static ConstructorHelpers::FClassFinder<UCorporisHUDWidget> UI_HUD_C(TEXT("/Game/Blueprints/UI/BP_UI_HUD.BP_UI_HUD_C"));
    if (UI_HUD_C.Succeeded())
        HUDWidgetClass = UI_HUD_C.Class;
}

void ACorporisPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    HUDWidget = CreateWidget<UCorporisHUDWidget>(this, HUDWidgetClass);
    HUDWidget->AddToViewport();
}
