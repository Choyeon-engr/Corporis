#include "CorporisPauseWidget.h"
#include "CorporisPlayerController.h"
#include "Components/Button.h"

void UCorporisPauseWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    ContinueBtn = Cast<UButton>(GetWidgetFromName(TEXT("ContinueButton")));
    ContinueBtn->OnClicked.AddDynamic(this, &UCorporisPauseWidget::OnContinueClicked);
}

void UCorporisPauseWidget::OnContinueClicked()
{
    auto CorporisPlayerController = Cast<ACorporisPlayerController>(GetOwningPlayer());
    
    RemoveFromParent();
    
    CorporisPlayerController->ChangeInputMode(true);
    CorporisPlayerController->SetPause(false);
}
