#include "GProjectile.h"
#include "IronEaterCharacter.h"
#include "KillerEater.h"
#include "Ghost.h"
#include "CreatureRadiationBug.h"
#include "WormCharacter.h"
#include "CreatureSoftCharacter.h"
#include "SpiderCharacter.h"
#include "GPSTargetObjectActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GPSFlameExFire.h"
#include "BlackforceCharacter.h"
#include "Animation/AnimationAsset.h"
#include "GGPlayerState.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"

    // Default Ayarlar
    AGProjectile::AGProjectile()
    {
		bReplicates = true;
    
			// Temel collision ayarları
			CollisionComp = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
			CollisionComp->InitSphereRadius(5.0f);
			CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

			if (GetLocalRole() == ROLE_Authority)
			{
				CollisionComp->OnComponentHit.AddDynamic(this, &AGProjectile::OnHit);	// Isabet sonrası çağırılacak fonksiyon
			}
     
      
			// Player üzerinde yürüyemez
			CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
			CollisionComp->CanCharacterStepUpOn = ECB_No;

			//Kök component
			RootComponent = CollisionComp;

			// Projectile Hareket Ayarları
			ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileComp"));
			ProjectileMovement->UpdatedComponent = CollisionComp;
			ProjectileMovement->InitialSpeed = 20000.f;
			//ProjectileMovement->MaxSpeed = 3000.f;
			ProjectileMovement->bRotationFollowsVelocity = true;
			ProjectileMovement->bShouldBounce = false;
		

			Bullet = CreateDefaultSubobject<UStaticMeshComponent>(FName("BulletMesh"));
			Bullet->SetupAttachment(RootComponent);
			
			//
			//Light = CreateDefaultSubobject<UParticleSystemComponent>(FName("Light"));
			//Light->AttachTo(RootComponent);


//Silahtan çıkan mermi mi başka bir şey mi? Buna göre yaşam süresi ve yer çekimi ayarını belirleyin.
			if (IsBullet == true)
			{
				InitialLifeSpan = 10.0f;
				ProjectileMovement->ProjectileGravityScale = 0.0f;
			}

			else
			{
				InitialLifeSpan = 2.0f;
				ProjectileMovement->ProjectileGravityScale = 0.4f;
			}


			
	}


	//void AGProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
	//{
	//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//

	//	DOREPLIFETIME(AGeleceginGunluguProjectile, DamageAmmount);
	//
	//}



//Merminin isabet ettiği karakteri tespit eder. İsabete sahip oyuncuya puan verir. Isabete göre zarar çağrılır.
void AGProjectile::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	
	if (IsBullet == true)
	{
		

		if (ABlackforceCharacter* HitPlayerB = Cast< ABlackforceCharacter>(Hit.Actor))
		{
			if (nullptr != HitPlayerB)
			{
				HitValidComponent = true;
				if (GGState)
				{

					GGState->SetPlayerScore(100);
				}

				FPointDamageEvent damageEventB;
				damageEventB.HitInfo = Hit;
				HitPlayerB->TakeDamage(DamageAmmount, damageEventB, GetInstigatorController(), this);
				if (GGCPlayer)
				{
					GGCPlayer->HitMarkShower();
				}
			}



		}

		else if (AIronEaterCharacter* HitPlayerC = Cast< AIronEaterCharacter>(Hit.Actor))
		{
			if (nullptr != HitPlayerC)
			{
				HitValidComponent = true;
				if (GGState)
				{

					GGState->SetPlayerScore(100);
				}

				FPointDamageEvent damageEventC;
				damageEventC.HitInfo = Hit;
				HitPlayerC->TakeDamage(DamageAmmount, damageEventC, GetInstigatorController(), this);
				if (GGCPlayer)
				{
					GGCPlayer->HitMarkShower();
				}
			}



		}

		else if (AKillerEater* HitPlayerK = Cast< AKillerEater>(Hit.Actor))
		{
			if (HitPlayerK->Health > 0)
			{

			
			HitValidComponent = true;
			if (GGState)
			{

				GGState->SetPlayerScore(100);
			}
			auto message = FString::Printf(TEXT("%i"), Point);
			GEngine->AddOnScreenDebugMessage(102, 5.0f, FColor::Green, message);
			FPointDamageEvent damageEventC;
			damageEventC.HitInfo = Hit;
			HitPlayerK->TakeDamage(DamageAmmount, damageEventC, GetInstigatorController(), this);
			if (GGCPlayer)
			{
				
				GGCPlayer->HitMarkShower();

			}
			}
		}


		else if (ACreatureRadiationBug* HitPlayerRad = Cast< ACreatureRadiationBug>(Hit.Actor))
		{
			HitValidComponent = true;
			if (GGState)
			{

				GGState->SetPlayerScore(100);
			}

			FPointDamageEvent damageEventC;
			damageEventC.HitInfo = Hit;
			HitPlayerRad->TakeDamage(40);
			if (GGCPlayer)
			{
				GGCPlayer->HitMarkShower();
			}
		}



		else if (AGhost* HitGhost = Cast<AGhost>(Hit.Actor))
		{
			HitValidComponent = true;
			if (GGState)
			{

				GGState->SetPlayerScore(200);
			}

			FPointDamageEvent damageEventC;
			damageEventC.HitInfo = Hit;
			HitGhost->TakeDamage(50);
			if (GGCPlayer)
			{
				GGCPlayer->HitMarkShower();
			}
		}

		else if (ACreatureSoftCharacter* HitPlayerTree = Cast< ACreatureSoftCharacter>(Hit.Actor))
		{
		HitValidComponent = true;
		if (GGState)
		{

			GGState->SetPlayerScore(100);
		}

		FPointDamageEvent damageEventC;
		damageEventC.HitInfo = Hit;
		HitPlayerTree->TakeDamage(10);

			if (GGCPlayer)
			{
				GGCPlayer->HitMarkShower();
			}
		}



		else if (ASpiderCharacter* HitSpider = Cast< ASpiderCharacter>(Hit.Actor))
		{
			if (nullptr != HitSpider)
			{
				HitValidComponent = true;
				if (GGState)
				{

					GGState->SetPlayerScore(100);
				}

				FPointDamageEvent damageEvent;
				damageEvent.HitInfo = Hit;
				HitSpider->TakeDamage(DamageAmmount);
				if (GGCPlayer)
				{
					GGCPlayer->HitMarkShower();
				}

			}



		}


		else if (AGPSTargetObjectActor* HitPlayerDummy = Cast< AGPSTargetObjectActor>(Hit.Actor))
		{

		HitValidComponent = true;
			if (HitPlayerDummy)

				if (GGState)
				{
					if (HitPlayerDummy->Red == true)
					{
						TargetRed = 1.0f;
						GGState->SetPlayerScore(100);

					}


					if(HitPlayerDummy->Green == true)
					{
						if (GGState->GetPlayerScore() != 0)
						{
							TargetGreen = 1.0f;
							GGState->SetPlayerScore(-100);

						}
					}

				}

			FPointDamageEvent damageEventC;
			damageEventC.HitInfo = Hit;
			HitPlayerDummy->KillActor(20);

			if (GGCPlayer)
				{
					GGCPlayer->HitMarkShower();
				}
			
		}

		else if (AWormCharacter* WormCha = Cast< AWormCharacter>(Hit.Actor))
		{
			WormCha->TakeDamage(DamageAmmount);

		if (GGCPlayer)
		{
			GGCPlayer->HitMarkShower();
		}

		}

		// Mermiyi yok et. Temas algısında kendisini imha eder. Yalnızca duvardan geçmeyen mermiler için kullan!

		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 1.0f, GetActorLocation());

			Destroy();
		}

		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
		{
			OtherActor->OnActorHit;
		}

		Destroy();
	}
  
  
	//Yangın Söndürücü için görünmez sihirli mermi fonksiyonu
	else
	{
		if (AGPSFlameExFire* Emitter = Cast<AGPSFlameExFire>(Hit.Actor))
		{
			Emitter->FireHealth();
		}


		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
		{
			OtherActor->OnActorHit;
		}
    
		Destroy();
	}
}

void AGProjectile::FoundOwnerActor(AActor* Actor)
{
	if (IsBullet == true)
	{
		GGCPlayer = Cast< AGeleceginGunluguCharacter>(Actor);
		if (AGeleceginGunluguCharacter* OwnerActor = Cast< AGeleceginGunluguCharacter>(Actor))
		{
			
			GGState = OwnerActor->GetPlayerState< AGGPlayerState>();
			if (OwnerActor->FP_Weapon)
			{
				DamageAmmount = 20;
			}

			else if (OwnerActor->FP_WeaponAK47)
			{
				DamageAmmount = 30;
			}


			else if (OwnerActor->FP_WeaponDarkGun)
			{
				DamageAmmount = 70;
			}


			else if (OwnerActor->FP_WeaponM4)
			{
				DamageAmmount = 30;
			}

			else if (OwnerActor->FP_WeaponPistol)
			{
				DamageAmmount = 7;
			}

			else if (OwnerActor->FP_WeaponSniper)
			{
				DamageAmmount = 500;
			}


			else if (OwnerActor->FP_WeaponShotgun)
			{
				DamageAmmount = 200;
			}


			else if (OwnerActor->FP_WeaponMp5)
			{
				DamageAmmount = 100;
			}

			else

			{
				DamageAmmount = 100;
			}
		}
	}

}
