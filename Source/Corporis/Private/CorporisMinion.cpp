// Fill out your copyright notice in the Description page of Project Settings.


#include "CorporisMinion.h"
#include "CorporisAIController.h"
#include "CorporisChampion.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ACorporisMinion::ACorporisMinion() : MinionHP(500), bOnSeePawn(false), bOnHearNoise(false), DeadTimer(1.0f), NextAttackTime(0)
{
     // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    GetMesh()->SetCollisionProfileName(TEXT("CorporisActor"));
    
    AIControllerClass = ACorporisAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
    
    PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
    PawnSensor->SensingInterval = 0.5f;
    PawnSensor->SightRadius = 3000.0f;
    PawnSensor->SetPeripheralVisionAngle(70.0f);
    
    static ConstructorHelpers::FObjectFinder<USoundWave> WEAPON_FIRE(TEXT("/Game/SciFiWeapLight/Sound/Rifle/Wavs/Rifle_Fire06"));
    if (WEAPON_FIRE.Succeeded())
        WeaponFireSoundWave = WEAPON_FIRE.Object;
    
    static ConstructorHelpers::FObjectFinder<UParticleSystem> WEAPON_MUZZLE(TEXT("/Game/ParagonMinions/FX/Particles/Minions/Shared/P_Minion_Seige_Muzzle"));
    if (WEAPON_MUZZLE.Succeeded())
        MuzzleParticleSystem = WEAPON_MUZZLE.Object;
    
    static ConstructorHelpers::FObjectFinder<UParticleSystem> MINION_DEATH(TEXT("/Game/ParagonMinions/FX/Particles/Minions/Minion_Siege/FX/Death/P_SiegeMinion_Chunks"));
    if (MINION_DEATH.Succeeded())
        DeathParticleSystem = MINION_DEATH.Object;
}

// Called when the game starts or when spawned
void ACorporisMinion::BeginPlay()
{
    Super::BeginPlay();
    
    UGameplayStatics::SpawnEmitterAttached(MuzzleParticleSystem, GetMesh(), FName("weapon_r"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true, EPSCPoolMethod::None, false);
    
    UGameplayStatics::SpawnEmitterAttached(MuzzleParticleSystem, GetMesh(), FName("spine_01"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true, EPSCPoolMethod::None, false);
}

// Called every frame
void ACorporisMinion::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ACorporisMinion::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    CorporisAnim = Cast<UCorporisAnimInstance>(GetMesh()->GetAnimInstance());
    
    CorporisAnim->MEnablePhysics.AddLambda([this]() -> void {
        UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticleSystem, GetMesh()->GetSocketLocation(FName(TEXT("spine_01"))), GetActorRotation());
        GetMesh()->SetSimulatePhysics(true);
        
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
            Destroy();
        }), DeadTimer, false);
    });
    
    PawnSensor->OnSeePawn.AddDynamic(this, &ACorporisMinion::OnSeePawn);
    PawnSensor->OnHearNoise.AddDynamic(this, &ACorporisMinion::OnHearNoise);
}

// Called to bind functionality to input
void ACorporisMinion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACorporisMinion::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    FHitResult HitResult;
    FVector ImpulseDirection;
    DamageEvent.GetBestHitInfo(this, DamageCauser, HitResult, ImpulseDirection);
    
    auto CorporisAIController = Cast<ACorporisAIController>(GetController());
    
    if (HitResult.BoneName == "head")
    {
        CorporisAIController->StopAI();
        
        MinionHP = 0;
        OnHPChanged.Broadcast();
        
        CorporisAnim->SetIsDead(true);
    }
    
    else
    {
        MinionHP -= DamageAmount;
        OnHPChanged.Broadcast();
        
        CorporisAnim->SetIsDamaged(true);
        
        if (MinionHP <= 0)
        {
            CorporisAIController->StopAI();
            CorporisAnim->SetIsDead(true);
        }
    }
    
    OnHPChanged.Broadcast();
    
    return FinalDamage;
}

void ACorporisMinion::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    
    GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void ACorporisMinion::Attack()
{
    if (NextAttackTime > GetWorld()->GetTimeSeconds())
    {
        CorporisAnim->SetAttackAngle(3);
        return;
    }
    
    NextAttackTime = GetWorld()->GetTimeSeconds() + 0.6f;
    
    FHitResult HitResult;
    FCollisionQueryParams Params(NAME_None, false, this);
    bool bResult = GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 2000.0f, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(500.0f), Params);
    auto Target = Cast<ACorporisChampion>(HitResult.Actor);
    
    if (bResult && Target)
    {
        if (Target->ChampionIsDead())
            CorporisAnim->SetAttackAngle(3);
        
        else
        {
            float HightGap = Target->GetActorLocation().Z - GetActorLocation().Z;
            
            if (HightGap > 200.0f)
                CorporisAnim->SetAttackAngle((int)EAngle::TOP);
            
            else if (HightGap < -200.0f)
                CorporisAnim->SetAttackAngle((int)EAngle::BTM);
            
            else
                CorporisAnim->SetAttackAngle((int)EAngle::MID);
            
            UGameplayStatics::SpawnEmitterAtLocation(this, MuzzleParticleSystem, GetMesh()->GetSocketLocation(FName(TEXT("Muzzle_Front"))), GetActorRotation());
            
            UGameplayStatics::SpawnSoundAtLocation(this, WeaponFireSoundWave, GetActorLocation(), GetActorRotation(), 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
            
            UGameplayStatics::ApplyPointDamage(Target, 50.0f, GetActorForwardVector(), HitResult, GetController(), this, nullptr);
        }
    }
    
    else
        CorporisAnim->SetAttackAngle(3);
}
