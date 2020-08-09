#pragma once

#include "Corporis.h"
#include "Blueprint/UserWidget.h"
#include "CorporisPauseWidget.generated.h"

UCLASS()
class CORPORIS_API UCorporisPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;
    
    UFUNCTION()
    void OnContinueClicked();
    
protected:
    UPROPERTY()
    class UButton* ContinueBtn;
};
