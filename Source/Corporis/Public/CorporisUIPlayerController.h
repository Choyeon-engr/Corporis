#pragma once

#include "Corporis.h"
#include "GameFramework/PlayerController.h"
#include "CorporisUIPlayerController.generated.h"

UCLASS()
class CORPORIS_API ACorporisUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;
    
private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = UI, Meta = (AllowPrivateAccess = true))
    TSubclassOf<class UUserWidget> UIWidgetClass;
    
    UPROPERTY()
    class UUserWidget* UIWidgetInstance;
};
