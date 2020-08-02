// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Corporis.h"
#include "Animation/AnimInstance.h"
#include "CorporisAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FCEnablePhysicsDelegate);
DECLARE_MULTICAST_DELEGATE(FMEnablePhysicsDelegate);
DECLARE_MULTICAST_DELEGATE(FReloadCompletedDelegate);

/**
 *
 */
UCLASS()
class CORPORIS_API UCorporisAnimInstance : public UAnimInstance
{
    GENERATED_BODY()
    
public:
    UCorporisAnimInstance();
    
public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    
    void SetIsDead(bool IsDead) { bIsDead = IsDead; }
    void SetAttackAngle(int AttackAngle) { iAttackAngle = AttackAngle; }
    void SetIsSit(bool IsSit) { bIsSit = IsSit; }
    void SetIsDamaged(bool IsDamaged) { bIsDamaged = IsDamaged; }
    
    void PlayReloadMontage();
    
    FCEnablePhysicsDelegate CEnablePhysics;
    FMEnablePhysicsDelegate MEnablePhysics;
    FReloadCompletedDelegate ReloadCompleted;
    
private:
    UFUNCTION()
    void AnimNotify_CEnablePhysics() { CEnablePhysics.Broadcast(); }
    
    UFUNCTION()
    void AnimNotify_MEnablePhysics() { MEnablePhysics.Broadcast(); }
    
    UFUNCTION()
    void AnimNotify_CFinishHitReact() { bIsDamaged = false; }
    
    UFUNCTION()
    void AnimNotify_MFinishHitReact() { bIsDamaged = false; }
    
    UFUNCTION()
    void AnimNotify_ReloadCompleted() { ReloadCompleted.Broadcast(); }
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
    float CurrentPawnSpeed;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
    bool bIsDead;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
    int iAttackAngle;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
    bool bIsSit;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
    bool bIsDamaged;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Reload, Meta = (AllowPrivateAccess = true))
    UAnimMontage* ReloadMontage;
};
