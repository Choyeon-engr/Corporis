#include "CorporisAnimInstance.h"

UCorporisAnimInstance::UCorporisAnimInstance() : CurrentPawnSpeed(0.0f), bIsDead(false), bIsDamaged(false), iAttackAngle(3)
{
    static ConstructorHelpers::FObjectFinder<UAnimMontage> RELOAD_MONTAGE(TEXT("/Game/ParagonLtBelica/Characters/Heroes/Belica/Animations/Reload"));
    if (RELOAD_MONTAGE.Succeeded())
        ReloadMontage = RELOAD_MONTAGE.Object;
}

void UCorporisAnimInstance::PlayReloadMontage()
{
    if (!Montage_IsPlaying(ReloadMontage))
        Montage_Play(ReloadMontage, 1.0f);
}

void UCorporisAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    
    auto Pawn = TryGetPawnOwner();
    if (::IsValid(Pawn)) { CurrentPawnSpeed = Pawn->GetVelocity().Size(); }
}
