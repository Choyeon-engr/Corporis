// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Corporis.h"
#include "GameFramework/Character.h"
#include "CorporisAnimInstance.h"
#include "CorporisMinion.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSeePawnDelegate, APawn*, Pawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHearNoiseDelegate, APawn*, Instigator, const FVector &, Location, float, Volume);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS()
class CORPORIS_API ACorporisMinion : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ACorporisMinion();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void PostInitializeComponents() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
    
    virtual void PossessedBy(AController* NewController) override;
    
    void Attack();
    bool GetOnSeePawn() const   { return bOnSeePawn; };
    bool GetOnHearNoise() const { return bOnHearNoise; };
    void SetOnSeePawn(bool OnSeePawn)       { bOnSeePawn = OnSeePawn; }
    void SetOnHearNoise(bool OnHearNoise)   { bOnHearNoise = OnHearNoise; }
    
    FOnHPChangedDelegate OnHPChanged;
    
private:
    UFUNCTION()
    void OnSeePawn(APawn* OtherPawn) { bOnSeePawn = true; }
    
    UFUNCTION()
    void OnHearNoise(APawn* OtherActor, const FVector & Location, float Volume) { bOnHearNoise = true; }
    
private:
    int32 MinionHP;
    bool bOnSeePawn;
    bool bOnHearNoise;
    float DeadTimer;
    float NextAttackTime;
    
    FTimerHandle DeadTimerHandle = { };
    
    enum class EAngle
    {
        TOP = 0,
        MID,
        BTM
    };
    
    UPROPERTY()
    UCorporisAnimInstance* CorporisAnim;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Awareness, Meta = (AllowPrivateAccess = true))
    class UPawnSensingComponent* PawnSensor;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Noise, Meta = (AllowPrivateAccess = true))
    class USoundWave* WeaponFireSoundWave;
};
