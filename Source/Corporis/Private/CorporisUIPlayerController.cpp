#include "CorporisUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void ACorporisUIPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
    
    UIWidgetInstance->AddToViewport();
    
    FInputModeUIOnly UIMode;
    UIMode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
    SetInputMode(UIMode);
    bShowMouseCursor = true;
}
