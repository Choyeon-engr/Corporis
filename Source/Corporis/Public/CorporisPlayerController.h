#pragma once

#include "Corporis.h"
#include "GameFramework/PlayerController.h"
#include "CorporisHUDWidget.h"
#include "CorporisDeathWidget.h"
#include "CorporisPlayerController.generated.h"

UCLASS()
class CORPORIS_API ACorporisPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    ACorporisPlayerController();
    
    UCorporisHUDWidget* GetHUDWidget() const { return HUDWidget; }
    UCorporisDeathWidget* GetDeathWidget() const { return DeathWidget; }
    
    void ChangeInputMode(bool bIsGameMode = true);
    
    void ShowDeathUI();
    
protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    
private:
    void Pause();
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
    TSubclassOf<class UCorporisHUDWidget> HUDWidgetClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
    TSubclassOf<class UCorporisDeathWidget> DeathWidgetClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
    TSubclassOf<class UCorporisPauseWidget> PauseWidgetClass;
    
private:
    UPROPERTY()
    UCorporisHUDWidget* HUDWidget;
    
    UPROPERTY()
    UCorporisDeathWidget* DeathWidget;
    
    UPROPERTY()
    class UCorporisPauseWidget* PauseWidget;
    
    FInputModeGameOnly GameMode;
    FInputModeUIOnly UIMode;
};
