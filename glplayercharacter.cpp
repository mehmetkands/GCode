#include "GeleceginGunluguCharacter.h"
#include "SinglePlayerCampaing1.h"
#include "Animation/AnimationAsset.h"
#include "GeleceginGunluguHUD.h"
#include "GeleceginGunluguProjectile.h"
#include "GGCPlayerController.h"
#include "GGGameStateBase.h"
#include "Animation/AnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Containers/Array.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Pawn.h"
#include "InterActiveTextWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "WeaponSmgUmp.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Objects.h"
#include "GGPlayerState.h"
#include "GGCAIController.h"
#include "WeaponSmgUmp.h"
#include "WeaponUmp.h"
#include "DarkGun.h"
#include "WeaponShotgun.h"
#include "Components/PanelWidget.h"
#include "WeaponSniper.h"
#include "Blueprint/UserWidget.h"
#include "GenericTeamAgentInterface.h"
#include "MotionControllerComponent.h"
#include "Curves/CurveLinearColor.h"
#include "ScoreAnalysis.h"
#include "SpawnLocationBox.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h" 
#include "GameFramework/PlayerController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);


***********************************************//etc*******************************************************************

// Silah Değiştirme*** Aktif slot numarasına göre silahı yok eder, ikincil silah numarasına göre yeni silahı spawn eder.//
void AGeleceginGunluguCharacter::ChangeSystem()
{
	if (FP_EqpHealSyringe != NULL)
	{
		FP_EqpHealSyringe->Destroy();
		FP_EqpHealSyringe = NULL;
	}

	if (currentweaponslot == 0)
	{
		CurrentSlotNumber = 1;
		currentweaponslot = 1;
		secondweaponslot = 0;

		CurrentWeapon = FirstWeaponNumber;

		if (CurrentWeapon != 2.f)
		{
			if (FP_Weapon != NULL)
			{
				WeaponUMPNumBool = 0;
				FP_Weapon->Destroy();
				FP_Weapon = NULL;
			}
		}

		else if (CurrentWeapon != 3)
		{
			if (FP_WeaponAK47 != NULL)
			{
				WeaponAK47NumBool = 0;
				FP_WeaponAK47->Destroy();
				FP_WeaponAK47 = NULL;
			}
		}

	else if (CurrentWeapon != 4.f)
		{
			if (FP_WeaponDarkGun != NULL)
			{
				FP_WeaponDarkGun->Destroy();
				WeaponDarkGunNumBool = 0;
				FP_WeaponDarkGun = NULL;
			}
		}

		else if (CurrentWeapon != 5)
		{
			if (FP_WeaponSniper != NULL)
			{
				WeaponSniperNumBool = 0;
				FP_WeaponSniper->Destroy();
				FP_WeaponSniper = NULL;
			}
		}

		else if (CurrentWeapon != 6)
		{
			if (FP_WeaponShotgun != NULL)
			{
				WeaponShotgunNumBool = 0;
				FP_WeaponShotgun->Destroy();
				FP_WeaponShotgun = NULL;
			}
		}

		if (CurrentWeapon != 7)
		{
			if (FP_WeaponM4 != NULL)
			{
				WeaponM4NumBool = 0;
				FP_WeaponM4->Destroy();
				FP_WeaponM4 = NULL;
			}
		}


		else if (CurrentWeapon != 8)
		{
			if (FP_WeaponPistol != NULL)
			{
				WeaponPistolNumBool = 0;
				FP_WeaponPistol->Destroy();
				FP_WeaponPistol = NULL;
			}
		}

	else if (CurrentWeapon != 9)
	{
		if (FP_FireExtinguisher)
		{
			WeaponExtNumBool = 0;
			FP_FireExtinguisher->Destroy();
		
		}
	}

	else if (CurrentWeapon != 10)
	{
		if (FP_WeaponMp5!=NULL)
		{
			WeaponMP5KNumBool = 0;
			FP_WeaponMp5->Destroy();
		}
	}
 
	//Lokasyon ve rotasyona göre spawn edici sistem
		if (CurrentWeapon == 2.f)
		{
			WeaponUMPNumBool = 1;
			FPGun->SetRelativeLocation(FVector(YT));
			FPGun->SetRelativeRotation(FRotator(YR));
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			const FRotator WeaponSpawnRotation = FPGun->GetComponentRotation();
      
			const FVector WeaponSpawnLocation = FPGun->GetComponentLocation();

			FP_Weapon = GetWorld()->SpawnActor<AWeaponSmgUmp>(FpsWeapon, WeaponSpawnLocation, WeaponSpawnRotation, ActorSpawnParams);
			FP_Weapon->EQPAnim();
			WeaponName = FP_Weapon->WeaponName;

			


		if (WeaponID2 == 1.0f)
		{
			Ammoleft = MemoryAmmo2;
			Have = MemoryHave2;
		}

		else if (WeaponID2 == 0.0f)
		{
			Ammoleft = FP_Weapon->WeaponAmmo;
			Have = FP_Weapon->WeaponHave;
			MemoryAmmo2 = Ammoleft;
			MemoryHave2 = Have;
			WeaponID2 = 1.0f;
		}
      
			if (FP_Weapon)
			{
				FP_Weapon->AttachToComponent(FPGun, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Bone001"));
			}
		}
}

//Hasar sistemi, Kemige dayalı çalışır, tanımlı her bir uzuv için farklı hasar miktarı verilebilir. 

float AGeleceginGunluguCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsLocallyControlled())
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DamageShake);
	}

		AGeleceginGunluguCharacter* TheplayerCharacter = Cast<AGeleceginGunluguCharacter>(this);
		AGGPlayerState* GGPS = TheplayerCharacter->GetPlayerState< AGGPlayerState>();

		float thedamage = PlayerHealth - Damage;
		SetCurrentHealth(thedamage);
  
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		{
			const auto pointDamageEvent = (FPointDamageEvent*)&DamageEvent;
			FName boneName = pointDamageEvent->HitInfo.BoneName;

			if (0 == boneName.Compare(FName(TEXT("head"))))
			{

				PlayerHealth = 0;
			}

			if (0 == boneName.Compare(FName(TEXT("spine_01"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("upperarm_l"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("lowerarm_l"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("hand_l"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("upperarm_r"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("lowerarm_r"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("hand_r"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}


			if (0 == boneName.Compare(FName(TEXT("thigh_l"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("calf_l"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("foot_l"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("ball_l"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("thigh_r"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("calf_r"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("foot_r"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}

			if (0 == boneName.Compare(FName(TEXT("ball_r"))))
			{

				PlayerHealth = PlayerHealth - Damage;
			}
		}
		OnRep_CurrentHealth();
	return thedamage;
}

//Nişans alma ADS sistemi Animasyonlarda lineer interpolasyon kullanır.

void AGeleceginGunluguCharacter::StartAim()
{
	AGeleceginGunluguHUD* HudUpdate = Cast<AGeleceginGunluguHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (HudUpdate)
	{
		HudUpdate->ShowCrosshair(); //Nişangahı gizler.
	}

	if (iscanfire == 1)
	{
		if (CurrentWeapon==2.f)
		{
			TimelineValue = AimTimeline.GetPlaybackPosition();
			AimTimeline.SetPlayRate(6.0f);
			if (FP_Weapon->RedDotSightValue == 1)
			{
				FPGun->SetRelativeLocation(FVector(FMath::Lerp(XTStock, YTStock, AimTimeline.GetPlaybackPosition())));
				FPGun->SetRelativeRotation(FRotator(FMath::Lerp(XRStock, YRStock, AimTimeline.GetPlaybackPosition())));
				FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(90.0f, 70.0f, AimTimeline.GetPlaybackPosition()));
			}
      
			else if (FP_Weapon->ScopeSight == 1)
			{
				FPGun->SetRelativeLocation(FVector(FMath::Lerp(XTScope, YTScope, AimTimeline.GetPlaybackPosition())));
				FPGun->SetRelativeRotation(FRotator(FMath::Lerp(XRScope, YRScope, AimTimeline.GetPlaybackPosition())));
				FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(90.0f, 70.0f, AimTimeline.GetPlaybackPosition()));
			}
      
			else if (FP_Weapon->ScopeSight == 0 && FP_Weapon->RedDotSightValue == 0)
			{
				FPGun->SetRelativeLocation(FVector(FMath::Lerp(XT, YT, AimTimeline.GetPlaybackPosition())));
				FPGun->SetRelativeRotation(FRotator(FMath::Lerp(XR, YR, AimTimeline.GetPlaybackPosition())));
				FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(90.0f, 70.0f, AimTimeline.GetPlaybackPosition()));
			}

		}

		if (CurrentWeapon == 3.0f)
		{
			if(FP_WeaponAK47->RedDotSightValue == 1)
			{
				FPGun->SetRelativeLocation(FVector(FMath::Lerp(umpT, umpET, AimTimeline.GetPlaybackPosition())));
				FPGun->SetRelativeRotation(FRotator(FMath::Lerp(umpR, umpER, AimTimeline.GetPlaybackPosition())));
				FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(90.0f, 40.0f, AimTimeline.GetPlaybackPosition()));
			}

			else if (FP_WeaponAK47->ScopeSight == 1)
			{
				FPGun->SetRelativeLocation(FVector(FMath::Lerp(umpTScope, umpETScope, AimTimeline.GetPlaybackPosition())));
				FPGun->SetRelativeRotation(FRotator(FMath::Lerp(umpRScope, umpERScope, AimTimeline.GetPlaybackPosition())));
				FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(90.0f, 40.0f, AimTimeline.GetPlaybackPosition()));
			}

			else if(FP_WeaponAK47->ScopeSight == 0&& FP_WeaponAK47->RedDotSightValue == 0)
			{
				FVector A(0.0f, 10.475f, -21.00f);
				FRotator B(0.1f, 0.0f, -90.0f);
				FPGun->SetRelativeLocation(FVector(FMath::Lerp(umpTStock, umpETStock, AimTimeline.GetPlaybackPosition())));
				FPGun->SetRelativeRotation(FRotator(FMath::Lerp(umpRStock, umpERStock, AimTimeline.GetPlaybackPosition())));
				FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(90.0f, 60.0f, AimTimeline.GetPlaybackPosition()));
			}
			
			TimelineValue = AimTimeline.GetPlaybackPosition();
			AimTimeline.SetPlayRate(6.0f);
		}
}
  
  
  
  //Değişilen silahı yere atar. Bu fonksiyon kayıtlı mermi bilgisini düşürülen silaha aktarmaz. Diğer fonksiyonu kullan
void AGeleceginGunluguCharacter::SpawnDroppedWeapons()
{
    
	if (WeaponUMPNumBool == 1)
	{
		
		if (WPHUMP)
		{
			FVector SpawnLoc = DroppedSpawnLoc->GetComponentLocation();
			FRotator SpawnRotation = DroppedSpawnLoc->GetComponentRotation();
      
			//Spawn Collision Ayarları
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			// Silahın Spawn Olması
			AWeaponPickUpHelper* WPHL = GetWorld()->SpawnActor<AWeaponPickUpHelper>(WPHUMP, SpawnLoc, SpawnRotation, ActorSpawnParams);

			if (WPHL)
			  {
			  	WPHL->ActorMesh->SetSimulatePhysics(true);
				  //WPHL->ActorMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
				  WPHL->ActorMesh->SetCollisionResponseToAllChannels(ECR_Block);
			  }
		}
	}

	else if (WeaponAK47NumBool == 1)
	{
		if (WPHAK47)
		{
			FVector SpawnLoc = DroppedSpawnLoc->GetComponentLocation();
			FRotator SpawnRotation = DroppedSpawnLoc->GetComponentRotation();

			//Spawn Collision Ayarları
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			// Silahın Spawn Olması
			AWeaponPickUpHelper* WPHL = GetWorld()->SpawnActor<AWeaponPickUpHelper>(WPHAK47, SpawnLoc, SpawnRotation, ActorSpawnParams);

			  if (WPHL)
			  {
				  WPHL->ActorMesh->SetSimulatePhysics(true);
				  //WPHL->ActorMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			  	WPHL->ActorMesh->SetCollisionResponseToAllChannels(ECR_Block);
			  }
		}
}
   
