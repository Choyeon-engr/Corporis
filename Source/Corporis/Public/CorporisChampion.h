#pragma once

#include "Corporis.h"
#include "GameFramework/Character.h"
#include "CorporisAnimInstance.h"
#include "CorporisPlayerController.h"
#include "ParticleDefinitions.h"
#include "CorporisChampion.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBulletQuantityChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnScoreChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnKillInfoChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDeathInfoChangedDelegate);

UCLASS()
class CORPORIS_API ACorporisChampion : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ACorporisChampion();
    
    bool ChampionIsDead() const { return ChampionHP <= 0; }
    
    float GetHPRatio() { return ((ChampionHP < KINDA_SMALL_NUMBER) ? 0.0f : (ChampionHP / 800.0f)); }
    int32 GetBulletQuantity() const { return BulletQuantity; }
    int32 GetCurrentScore() const { return CurrentScore; }
    int32 GetHighScore() const { return HighScore; }
    FString GetKillInfo() const { return KillInfo; }
    FString GetDeathInfo() const { return DeathInfo; }
    
    void AddCurrentScore();
    void SavePlayerData();
    
    void SetKillInfo(FString Kill);

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
    void Attack();
    
public:
    FOnHPChangedDelegate OnHPChanged;
    FOnBulletQuantityChangedDelegate OnBulletQuantityChanged;
    FOnScoreChangedDelegate OnScoreChanged;
    FOnKillInfoChangedDelegate OnKillInfoChanged;
    FOnDeathInfoChangedDelegate OnDeathInfoChanged;

private:
    int32 ChampionHP;
    int32 BulletQuantity;
    int32 CurrentScore;
    int32 HighScore;
    FString KillInfo;
    FString DeathInfo;
    FString SaveSlotName;
    float DeadTimer;
    float ReloadTimer;
    float ShowUITimer;
    float LastFootstep;
    
    FTimerHandle DeadTimerHandle = { };
    FTimerHandle ReloadTimerHandle = { };
    FTimerHandle ShowUITimerHandle = { };
    
    UPROPERTY()
    UCorporisAnimInstance* CorporisAnim;
    
    UPROPERTY()
    ACorporisPlayerController* CorporisPlayerController;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
    class UPawnNoiseEmitterComponent* NoiseEmitter;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound, Meta = (AllowPrivateAccess = true))
    class USoundWave* WeaponFireSoundWave;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound, Meta = (AllowPrivateAccess = true))
    class USoundWave* WeaponReloadSoundWave;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound, Meta = (AllowPrivateAccess = true))
    class USoundWave* ImpactBodySoundWave;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = true))
    UParticleSystem* MuzzleParticleSystem;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = true))
    TSubclassOf<UCameraShake> CameraShake;
};
