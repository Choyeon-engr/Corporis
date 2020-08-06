#include "CorporisGameMode.h"
#include "CorporisChampion.h"
#include "CorporisPlayerController.h"

ACorporisGameMode::ACorporisGameMode()
{
    static ConstructorHelpers::FClassFinder<ACorporisChampion> PLAYER_CHAMPION_C(TEXT("/Game/ParagonLtBelica/Characters/Heroes/Belica/LtBelicaPlayerCharacter.LtBelicaPlayerCharacter_C"));
    if (PLAYER_CHAMPION_C.Succeeded())
        DefaultPawnClass = PLAYER_CHAMPION_C.Class;
    
    PlayerControllerClass = ACorporisPlayerController::StaticClass();
}
