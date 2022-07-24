#include "Health.h"
#include "Kismet/GameplayStatics.h"
#include "GeleceginGunluguCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AHealth::AHealth()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Character"));

}

// Called when the game starts or when spawned
void AHealth::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AHealth::AddHealth);
	Mesh->OnComponentEndOverlap.AddDynamic(this, &AHealth::SAddHealth);
	
}

// Called every frame
void AHealth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealth::AddHealth(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHealth == true && bAmmo == false)
	{
		HealthValue;
		if (AGeleceginGunluguCharacter* GGC = Cast <AGeleceginGunluguCharacter>(OtherActor))
		{
			if (GGC->HealthOverride < 100)
			{
				GGC->AddHealth(HealthValue);
        
				Destroy();
			}
		}
	}

	if (bAmmo == true && bHealth == false)
	{
		
		if (AGeleceginGunluguCharacter* GGC = Cast <AGeleceginGunluguCharacter>(OtherActor))
		{
			if (GGC->CurrentWeapon == 2)
			{
				if (GGC->Have < 300)
				{
					GGC->Have =GGC->Have + Ammo;

					if (GGC->Have > 300)
					{
						float Plus;
						Plus = GGC->Have - 300;
						GGC->Have = GGC->Have - Plus;
					}
					Destroy();
				}
			}

			if (GGC->CurrentWeapon == 3)
			{
				if (GGC->Have < 300)
				{
					GGC->Have = GGC->Have + Ammo;

					if (GGC->Have > 300)
					{
						float Plus;
						Plus = GGC->Have - 300;
						GGC->Have = GGC->Have - Plus;
					}
					Destroy();
				}
			}

			if (GGC->CurrentWeapon == 4)
			{
				if (GGC->Have < 1000)
				{
					GGC->Have = GGC->Have + Ammo;

					if (GGC->Have > 1000)
					{
						float Plus;
						Plus = GGC->Have - 1000;
						GGC->Have = GGC->Have - Plus;
					}
					Destroy();
				}
			}

			if (GGC->CurrentWeapon == 5)
			{
				if (GGC->Have < 90)
				{
					GGC->Have = GGC->Have + Ammo;

					if (GGC->Have > 90)
					{
						float Plus;
						Plus = GGC->Have - 90;
						GGC->Have = GGC->Have - Plus;
					}
					Destroy();
				}
			}

			if (GGC->CurrentWeapon == 6)
			{
				if (GGC->Have < 60)
				{
					GGC->Have = GGC->Have + Ammo;

					if (GGC->Have > 60)
					{
						float Plus;
						Plus = GGC->Have - 60;
						GGC->Have = GGC->Have - Plus;
					}
					Destroy();
				}
			}

			if (GGC->CurrentWeapon == 7)
			{
				if (GGC->Have < 300)
				{
					GGC->Have = GGC->Have + Ammo;

					if (GGC->Have > 300)
					{
						float Plus;
						Plus = GGC->Have - 300;
						GGC->Have = GGC->Have - Plus;
					}
					Destroy();
				}
			}

			if (GGC->CurrentWeapon == 8)
			{
				if (GGC->Have < 90)
				{
					GGC->Have = GGC->Have + Ammo;

					if (GGC->Have > 90)
					{
						float Plus;
						Plus = GGC->Have - 90;
						GGC->Have = GGC->Have - Plus;
					}
					Destroy();
				}
			}

			if (GGC->CurrentWeapon == 10)
			{
				if (GGC->Have < 300)
				{
					GGC->Have = GGC->Have + Ammo;

					if (GGC->Have>300)
					{
						float Plus;
						Plus = GGC->Have-300;
						GGC->Have = GGC->Have - Plus;
					}

					Destroy();
				}
			}
		}
	}
}
