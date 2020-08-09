#pragma once

#include "Corporis.h"
#include "GameFramework/PlayerController.h"
#include "CorporisHUDWidget.h"
#include "CorporisPlayerController.generated.h"

UCLASS()
class CORPORIS_API ACorporisPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    ACorporisPlayerController();
    
    UCorporisHUDWidget* GetHUDWidget() const { return HUDWidget; }
    
    void ChangeInputMode(bool bIsGameMode = true);
    
protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    
private:
    void Pause();
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
    TSubclassOf<class UCorporisHUDWidget> HUDWidgetClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
    TSubclassOf<class UCorporisPauseWidget> PauseWidgetClass;
    
private:
    UPROPERTY()
    UCorporisHUDWidget* HUDWidget;
    
    UPROPERTY()
    class UCorporisPauseWidget* PauseWidget;
    
    FInputModeGameOnly GameMode;
    FInputModeUIOnly UIMode;
};
