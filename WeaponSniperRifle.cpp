#include "WeaponSniper.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "GeleceginGunluguCharacter.h"

AWeaponSniper::AWeaponSniper(const FObjectInitializer& object_initializer)
{
	WeaponName = "Sniper Rifle";
	WeaponAmmo = 10.0f;
	WeaponHave = 90.0f;
	CharacterIswalk = 0.0f;
	CharacterIsRight = 0.0f;
	RedDotSightValue = 0;
	ScopeSight = 0;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponDir(TEXT("/Game/Weapons/Assets/Models/Rifles/Sniper/SniperWalkfbx"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RedDotSight(TEXT("/Game/Weapons/Assets/Models/Rifles/EQP/Dot/EOTech_Xps2-1"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RedScopeSight(TEXT("/Game/Weapons/Assets/Models/Rifles/EQP/Scope/model"));

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> WeaponAnimBPObj(TEXT("/Game/Weapons/Assets/Models/Rifles/Sniper/Sniper_AnimBp"));
	SprinArms = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	WeaponMesh->SetupAttachment(SprinArms);
	WeaponM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponEqp"));
	WeaponM->AttachToComponent(WeaponMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), ("sniperSocket"));

	ArmEqp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmEqp"));
	ArmEqp->AttachToComponent(WeaponMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), ("clocksocket"));


	if (RedDotSightValue == 1)
	{
		if (RedDotSight.Succeeded())
		{
			RedDotSightS = RedDotSight.Object;
		}
	}

	if (ScopeSight == 1)
	{
		if (RedScopeSight.Succeeded())
		{
			RedScopeSightS = RedScopeSight.Object;
		}
	}


	if (WeaponDir.Succeeded())
	{
		SKMesh = WeaponDir.Object;
	}



	//** ANIMATION BP **//
	if (WeaponAnimBPObj.Succeeded())
	{
		MeshAnimBP = WeaponAnimBPObj.Object;
	}

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(WeaponMesh);
	MuzzleLocation->SetRelativeLocation(FVector(21.0f, 71.0f, 10.0f));
	MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));


	Muzzlefx = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleFX"));
	Muzzlefx->SetupAttachment(WeaponMesh);
	Muzzlefx->SetRelativeLocation(FVector(21.0f, 71.0f, 16.0f));

	//WeaponM->SetStaticMesh(RedDotSight);
	//WeaponM->SetupAttachment(WeaponMesh, "RedDot");


	WeaponMesh->SetSkeletalMesh(SKMesh);
	WeaponMesh->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
	WeaponMesh->SetAnimInstanceClass(MeshAnimBP->GeneratedClass);



	if (RedDotSightValue == 1)
	{

		WeaponM->SetStaticMesh(RedDotSightS);
		if (WeaponM)
		{
			WeaponM->AttachToComponent(WeaponMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), ("sniperSocket"));
		}

	}


	if (ScopeSight == 1)
	{
		WeaponM->SetStaticMesh(RedScopeSightS);
		if (WeaponM)
		{
			WeaponM->AttachToComponent(WeaponMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), ("sniperSocket"));
		}
	}
}

// Called when the game starts or when spawned
void AWeaponSniper::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponSniper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponSniper::EQPAnim()
{
	if (EQPAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(EQPAnimation, 1.f);
		}
	}
}

void AWeaponSniper::Firesound()
{
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void AWeaponSniper::Memory()
{
	WeaponAmmo = 10;
	WeaponHave = 90;
}

void AWeaponSniper::Update()
{
	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 2.f);
		}
	}
}

void AWeaponSniper::ReloadAnim()
{
	if (ReloadAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(ReloadAnimation, 1.f);
		}
	}
}

void AWeaponSniper::ReloadEmptyAnim()
{
	if (ReloadEmptyAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(ReloadEmptyAnimation, 1.f);
		}
	}
}

void AWeaponSniper::RunAnim()
{
	if (RunAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(RunAnimation, 2.f);
		}
	}
}
