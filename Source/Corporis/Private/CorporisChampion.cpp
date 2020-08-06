#include "CorporisChampion.h"
#include "CorporisMinion.h"
#include "CorporisSaveGame.h"
#include "Components/PawnNoiseEmitterComponent.h"

// Sets default values
ACorporisChampion::ACorporisChampion() : ChampionHP(800), BulletQuantity(8), CurrentScore(0), HighScore(0),  KillInfo(TEXT("")), DeathInfo(TEXT("")), SaveSlotName(TEXT("Guest")), DeadTimer(0.03f), ReloadTimer(2.55f), ShowUITimer(1.0f), LastFootstep(0.0f)
{
     // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    GetMesh()->SetCollisionProfileName(TEXT("CorporisActor"));
    
    NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));
    
    static ConstructorHelpers::FObjectFinder<USoundWave> WEAPON_FIRE(TEXT("/Game/SciFiWeapDark/Sound/Rifle/Wavs/RifleA_Fire06"));
    if (WEAPON_FIRE.Succeeded())
        WeaponFireSoundWave = WEAPON_FIRE.Object;
    
    static ConstructorHelpers::FObjectFinder<USoundWave> WEAPON_RELOAD(TEXT("/Game/SciFiWeapDark/Sound/Rifle/Wavs/Rifle_Reload03"));
    if (WEAPON_RELOAD.Succeeded())
        WeaponReloadSoundWave = WEAPON_RELOAD.Object;
    
    static ConstructorHelpers::FObjectFinder<USoundWave> IMPACT_BODY(TEXT("/Game/SciFiWeapDark/Sound/Rifle/Wavs/Rifle_ImpactBody04"));
    if (IMPACT_BODY.Succeeded())
        ImpactBodySoundWave = IMPACT_BODY.Object;
    
    static ConstructorHelpers::FObjectFinder<UParticleSystem> WEAPON_MUZZLE(TEXT("/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/Primary/FX/P_BelicaMuzzle"));
    if (WEAPON_MUZZLE.Succeeded())
        MuzzleParticleSystem = WEAPON_MUZZLE.Object;
    
    auto CorporisSaveGame = Cast<UCorporisSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    if (!CorporisSaveGame)
        CorporisSaveGame = GetMutableDefault<UCorporisSaveGame>();
    
    HighScore = CorporisSaveGame->GetHighScore();
    SavePlayerData();
}

void ACorporisChampion::AddCurrentScore()
{
    if (++CurrentScore > HighScore) { ++HighScore; }
    OnScoreChanged.Broadcast();
    SavePlayerData();
}

void ACorporisChampion::SavePlayerData()
{
    UCorporisSaveGame* NewPlayerData = NewObject<UCorporisSaveGame>();
    NewPlayerData->SetHighScore(HighScore);
    UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0);
}

void ACorporisChampion::SetKillInfo(FString Kill)
{
    KillInfo = Kill;
    OnKillInfoChanged.Broadcast();
    GetWorld()->GetTimerManager().SetTimer(ShowUITimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
        KillInfo = TEXT("");
        OnKillInfoChanged.Broadcast();
    }), ShowUITimer, false);
}

// Called when the game starts or when spawned
void ACorporisChampion::BeginPlay()
{
    Super::BeginPlay();
    
    CorporisPlayerController = Cast<ACorporisPlayerController>(GetWorld()->GetFirstPlayerController());
    
    CorporisPlayerController->GetHUDWidget()->BindChampionStat(this);
    
    UGameplayStatics::SpawnEmitterAttached(MuzzleParticleSystem, GetMesh(), FName("weapon"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true, EPSCPoolMethod::None, false);
}

void ACorporisChampion::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    CorporisAnim = Cast<UCorporisAnimInstance>(GetMesh()->GetAnimInstance());
    
    CorporisAnim->CEnablePhysics.AddLambda([this]() -> void {
        GetMesh()->SetSimulatePhysics(true);
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void { Destroy(); }), DeadTimer, false);
    });
};

void ACorporisChampion::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

// Called every frame
void ACorporisChampion::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector VelocityVector = GetCharacterMovement()->Velocity;
    float VectorMagnitude = VelocityVector.Size();
    
    float Now = GetWorld()->GetTimeSeconds();

    if (Now > LastFootstep + 1.0f && VectorMagnitude > 0.0f && !GetCharacterMovement()->IsCrouching())
    {
        MakeNoise(0.5f, this, FVector::ZeroVector);
        LastFootstep = Now;
    }
}

// Called to bind functionality to input
void ACorporisChampion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ACorporisChampion::Attack);
}

float ACorporisChampion::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    FHitResult HitResult;
    FVector ImpulseDirection;
    DamageEvent.GetBestHitInfo(this, DamageCauser, HitResult, ImpulseDirection);
    
    UGameplayStatics::SpawnSoundAtLocation(this, ImpactBodySoundWave, GetActorLocation(), GetActorRotation(), 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
    
    CorporisPlayerController->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);
    
    if (HitResult.BoneName == "head")
    {
        ChampionHP = 0;
        OnHPChanged.Broadcast();
        
        DisableInput(CorporisPlayerController);
        CorporisAnim->SetIsDead(true);
        
        DeathInfo = TEXT("Be killed with a headshot!");
        OnDeathInfoChanged.Broadcast();
    }
    
    else
    {
        ChampionHP -= DamageAmount;
        OnHPChanged.Broadcast();
        
        CorporisAnim->SetIsDamaged(true);
        
        if (ChampionHP <= 0)
        {
            DisableInput(CorporisPlayerController);
            CorporisAnim->SetIsDead(true);
            
            DeathInfo = TEXT("Be killed!");
            OnDeathInfoChanged.Broadcast();
        }
    }
    
    return FinalDamage;
}

void ACorporisChampion::Attack()
{
    if (BulletQuantity < 0) { return; }
    
    FHitResult HitResult;
    FCollisionQueryParams Params(NAME_None, false, this);
    bool bResult = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 3000.0f, ECollisionChannel::ECC_GameTraceChannel1, Params);
    auto Target = Cast<ACorporisMinion>(HitResult.Actor);
    
    if (!BulletQuantity--)
    {
        CorporisAnim->PlayReloadMontage();
        UGameplayStatics::SpawnSoundAtLocation(this, WeaponReloadSoundWave, GetActorLocation(), GetActorRotation(), 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
        
        GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
            BulletQuantity = 8;
            OnBulletQuantityChanged.Broadcast();
        }), ReloadTimer, false);
        
        return;
    }
    
    OnBulletQuantityChanged.Broadcast();
    
    UGameplayStatics::SpawnEmitterAtLocation(this, MuzzleParticleSystem, GetMesh()->GetSocketLocation(FName(TEXT("CorporisPistol"))), GetActorRotation());
    
    UGameplayStatics::SpawnSoundAtLocation(this, WeaponFireSoundWave, GetActorLocation(), GetActorRotation(), 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
    MakeNoise(1.0, this, FVector::ZeroVector);
    
    GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);
    
    if (bResult && Target)
        UGameplayStatics::ApplyPointDamage(Target, 80.0f, GetActorForwardVector(), HitResult, GetController(), this, nullptr);
}
