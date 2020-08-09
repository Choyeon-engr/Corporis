#pragma once

#include "Corporis.h"
#include "Animation/AnimInstance.h"
#include "CorporisAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FCEnablePhysicsDelegate);
DECLARE_MULTICAST_DELEGATE(FMEnablePhysicsDelegate);
DECLARE_MULTICAST_DELEGATE(FReloadCompletedDelegate);

UCLASS()
class CORPORIS_API UCorporisAnimInstance : public UAnimInstance
{
    GENERATED_BODY()
    
public:
    UCorporisAnimInstance();
    
    void SetIsDead(bool IsDead) { bIsDead = IsDead; }
    void SetIsDamaged(bool IsDamaged) { bIsDamaged = IsDamaged; }
    void SetAttackAngle(int AttackAngle) { iAttackAngle = AttackAngle; }
    
    void PlayReloadMontage();
    
protected:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    
private:
    UFUNCTION()
    void AnimNotify_CEnablePhysics() { CEnablePhysics.Broadcast(); }
    
    UFUNCTION()
    void AnimNotify_MEnablePhysics() { MEnablePhysics.Broadcast(); }
    
    UFUNCTION()
    void AnimNotify_ReloadCompleted() { ReloadCompleted.Broadcast(); }
    
    UFUNCTION()
    void AnimNotify_CFinishHitReact() { bIsDamaged = false; }
    
    UFUNCTION()
    void AnimNotify_MFinishHitReact() { bIsDamaged = false; }
    
public:
    FCEnablePhysicsDelegate CEnablePhysics;
    FMEnablePhysicsDelegate MEnablePhysics;
    FReloadCompletedDelegate ReloadCompleted;
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
    float CurrentPawnSpeed;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
    bool bIsDead;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
    bool bIsDamaged;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
    int iAttackAngle;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Reload, Meta = (AllowPrivateAccess = true))
    UAnimMontage* ReloadMontage;
};
