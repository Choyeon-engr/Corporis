// Fill out your copyright notice in the Description page of Project Settings.


#include "CorporisAnimInstance.h"

UCorporisAnimInstance::UCorporisAnimInstance() : CurrentPawnSpeed(0.0f), bIsDead(false), iAttackAngle(3), bIsSit(false), bIsDamaged(false) { }

void UCorporisAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    
    auto Pawn = TryGetPawnOwner();
    if (::IsValid(Pawn)) { CurrentPawnSpeed = Pawn->GetVelocity().Size(); }
}
