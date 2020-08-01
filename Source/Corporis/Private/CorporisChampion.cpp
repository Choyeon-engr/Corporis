// Fill out your copyright notice in the Description page of Project Settings.


#include "CorporisChampion.h"
#include "CorporisMinion.h"
#include "Components/PawnNoiseEmitterComponent.h"

// Sets default values
ACorporisChampion::ACorporisChampion() : ChampionHP(8000), LastFootstep(0.0f), DeadTimer(0.03f)
{
     // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    GetMesh()->SetCollisionProfileName(TEXT("CorporisActor"));
    
    NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));
    
    static ConstructorHelpers::FObjectFinder<USoundWave> WEAPON_FIRE(TEXT("/Game/SciFiWeapDark/Sound/Rifle/Wavs/RifleA_Fire06"));
    if (WEAPON_FIRE.Succeeded())
        WeaponFireSoundWave = WEAPON_FIRE.Object;
    
    static ConstructorHelpers::FObjectFinder<USoundWave> IMPACT_BODY(TEXT("/Game/SciFiWeapDark/Sound/Rifle/Wavs/Rifle_ImpactBody04"));
    if (IMPACT_BODY.Succeeded())
        ImpactBodySoundWave = IMPACT_BODY.Object;
}

// Called when the game starts or when spawned
void ACorporisChampion::BeginPlay()
{
    Super::BeginPlay();
    
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

void ACorporisChampion::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    CorporisAnim = Cast<UCorporisAnimInstance>(GetMesh()->GetAnimInstance());
    
    CorporisAnim->CEnablePhysics.AddLambda([this]() -> void {
        GetMesh()->SetSimulatePhysics(true);
        
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
            Destroy();
        }), DeadTimer, false);
    });
}

// Called to bind functionality to input
void ACorporisChampion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ACorporisChampion::Attack);
    PlayerInputComponent->BindAction(TEXT("Sit"), EInputEvent::IE_Pressed, this, &ACorporisChampion::Sit);
    PlayerInputComponent->BindAction(TEXT("NotSit"), EInputEvent::IE_Released, this, &ACorporisChampion::Stand);
}

float ACorporisChampion::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    FHitResult HitResult;
    FVector ImpulseDirection;
    DamageEvent.GetBestHitInfo(this, DamageCauser, HitResult, ImpulseDirection);
    
    APlayerController* CorporisPlayerController = GetWorld()->GetFirstPlayerController();
    
    UGameplayStatics::SpawnSoundAtLocation(this, ImpactBodySoundWave, GetActorLocation(), GetActorRotation(), 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
    
    CorporisPlayerController->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);
    
    if (HitResult.BoneName == "head")
    {
        ChampionHP = 0;
        OnHPChanged.Broadcast();
        
        DisableInput(CorporisPlayerController);
        CorporisAnim->SetIsDead(true);
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
        }
    }
    
    return FinalDamage;
}

void ACorporisChampion::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

bool ACorporisChampion::ChampionIsDead() const
{
    if (ChampionHP <= 0) { return true; }
    
    return false;
}

float ACorporisChampion::GetHPRatio()
{
    return ((ChampionHP < KINDA_SMALL_NUMBER) ? 0.0f : (ChampionHP / 8000.0f));
}

void ACorporisChampion::Attack()
{
    FHitResult HitResult;
    FCollisionQueryParams Params(NAME_None, false, this);
    bool bResult = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 3000.0f, ECollisionChannel::ECC_GameTraceChannel1, Params);
    auto Target = Cast<ACorporisMinion>(HitResult.Actor);
    
    UGameplayStatics::SpawnSoundAtLocation(this, WeaponFireSoundWave, GetActorLocation(), GetActorRotation(), 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
    MakeNoise(1.0, this, FVector::ZeroVector);
    
    GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);
    
    if (bResult && Target)
        UGameplayStatics::ApplyPointDamage(Target, 80.0f, GetActorForwardVector(), HitResult, GetController(), this, nullptr);
}
