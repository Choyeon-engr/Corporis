#include "CorporisPlayerController.h"
#include "CorporisPauseWidget.h"

ACorporisPlayerController::ACorporisPlayerController()
{
    static ConstructorHelpers::FClassFinder<UCorporisHUDWidget> UI_HUD_C(TEXT("/Game/Blueprints/UI/BP_UI_HUD.BP_UI_HUD_C"));
    if (UI_HUD_C.Succeeded())
        HUDWidgetClass = UI_HUD_C.Class;
    
    static ConstructorHelpers::FClassFinder<UCorporisDeathWidget> UI_DEATH_C(TEXT("/Game/Blueprints/UI/BP_UI_Death.BP_UI_Death_C"));
    if (UI_DEATH_C.Succeeded())
        DeathWidgetClass = UI_DEATH_C.Class;
    
    static ConstructorHelpers::FClassFinder<UCorporisPauseWidget> UI_PAUSE_C(TEXT("/Game/Blueprints/UI/BP_UI_Pause.BP_UI_Pause_C"));
    if (UI_PAUSE_C.Succeeded())
        PauseWidgetClass = UI_PAUSE_C.Class;
}

void ACorporisPlayerController::ChangeInputMode(bool bIsGameMode)
{
    if (bIsGameMode)
    {
        SetInputMode(GameMode);
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(UIMode);
        bShowMouseCursor = true;
    }
}

void ACorporisPlayerController::ShowDeathUI()
{
    ChangeInputMode(false);
    
    DeathWidget->AddToViewport(1);
}

void ACorporisPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    ChangeInputMode(true);
    
    HUDWidget = CreateWidget<UCorporisHUDWidget>(this, HUDWidgetClass);
    HUDWidget->AddToViewport();
    
    DeathWidget = CreateWidget<UCorporisDeathWidget>(this, DeathWidgetClass);
}

void ACorporisPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    InputComponent->BindAction(TEXT("Pause"), EInputEvent::IE_Pressed, this, &ACorporisPlayerController::Pause);
}

void ACorporisPlayerController::Pause()
{
    PauseWidget = CreateWidget<UCorporisPauseWidget>(this, PauseWidgetClass);
    PauseWidget->AddToViewport(1);
    
    SetPause(true);
    ChangeInputMode(false);
}
