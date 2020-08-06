#pragma once

#include "Corporis.h"
#include "GameFramework/Character.h"
#include "CorporisAnimInstance.h"
#include "ParticleDefinitions.h"
#include "CorporisMinion.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSeePawnDelegate, APawn*, Pawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHearNoiseDelegate, APawn*, Instigator, const FVector &, Location, float, Volume);

UCLASS()
class CORPORIS_API ACorporisMinion : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ACorporisMinion();
    
    bool GetOnSeePawn() const   { return bOnSeePawn; };
    bool GetOnHearNoise() const { return bOnHearNoise; };
    
    void SetOnSeePawn(bool OnSeePawn)       { bOnSeePawn = OnSeePawn; }
    void SetOnHearNoise(bool OnHearNoise)   { bOnHearNoise = OnHearNoise; }
    
    void Attack();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void PostInitializeComponents() override;
    virtual void PossessedBy(AController* NewController) override;
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
    UFUNCTION()
    void OnSeePawn(APawn* OtherPawn) { bOnSeePawn = true; }
    
    UFUNCTION()
    void OnHearNoise(APawn* OtherActor, const FVector & Location, float Volume) { bOnHearNoise = true; }
    
public:
    FOnHPChangedDelegate OnHPChanged;
    
private:
    int32 MinionHP;
    bool bOnSeePawn;
    bool bOnHearNoise;
    bool bIsDead;
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
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = true))
    UParticleSystem* MuzzleParticleSystem;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = true))
    UParticleSystem* DeathParticleSystem;
};
