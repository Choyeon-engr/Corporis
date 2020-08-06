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
    
protected:
    virtual void BeginPlay() override;
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
    TSubclassOf<class UCorporisHUDWidget> HUDWidgetClass;
    
private:
    UPROPERTY()
    UCorporisHUDWidget* HUDWidget;
};
