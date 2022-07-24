#include "GGCEnvQueryGeneratorProjected.h"
#include "DrawDebugHelpers.h" 
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GGCAIController.h"
#include "blackboard_keys.h"
#include "GeleceginGunluguCharacter.h"
#include "BlackforceCharacter.h"

UGGCEnvQueryGeneratorProjected::UGGCEnvQueryGeneratorProjected()
{
	TempCoverPoint = (GetWorld(), CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Capsule")));

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	TriggerN = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerN"));
}
    
void UGGCEnvQueryGeneratorProjected::BeginPlay()
{
}

void UGGCEnvQueryGeneratorProjected::Tick(float DeltaTime)
{
}

void UGGCEnvQueryGeneratorProjected::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	//This array will hold a reference to all the generated items, meaning, the cone items
	TArray<FNavLocation> ItemCandidates;
	TArray<FNavLocation> WallCoverPoint;
	ABlackforceCharacter* BFC = Cast<ABlackforceCharacter>((QueryInstance.Owner).Get());

	AGGCAIController* GAI = Cast<AGGCAIController>(BFC->GetController());
	FVector AILoc = BFC->GetActorLocation();
	FVector FAILoc(1, 0, 0);
	float Linecount = 15;
	float forrot = 360.f;
	FVector BoxLoc;
	FVector NormalBox;

	FVector Origin;
	FVector	BoxExtent;
	
	//const FRotator Rot(0, 0, 0);
	//FVector AAILoc = Rot.RotateVector(FVector(FAILoc));

	//PlayerLocation == GGCActor->GetActorLocation();

	//Hit.Reset();


		for (int r = 0; r < forrot; r += 15)
		{
			const FRotator Rott(0, r, 0);
			//Rot == Rott;
			FVector AAILoc = Rott.RotateVector(FVector(FAILoc));
			
			for (AActor* Actor : GAI->DedectedPawn)
			{

			
			//FVector StartTrace = BFC->GetActorLocation();
			//FVector EndTrace = (BFC->GetActorForwardVector() * 2000) + StartTrace;


			FVector StartTracee = (AAILoc*3000)+ Actor->GetActorLocation();
			FVector EndTracee = Actor->GetActorLocation();

			


			//TArray<struct FHitResult> Hit;

			FHitResult Hit;
			

			TArray<FHitResult> MultiHit;
			MultiHit.Init(FHitResult(ForceInit), 10);
			FCollisionQueryParams* TraceParam = new FCollisionQueryParams();
			TraceParam->bTraceComplex = false;
			FCollisionResponseParams ResponseP(ECollisionResponse::ECR_Overlap);


			//GetWorld()->LineTraceSingleByChannel(Hit, StartTracee, EndTracee, ECC_Visibility);
			FCollisionShape Boyut = FCollisionShape::MakeSphere(2000.0f);
			GetWorld()->LineTraceSingleByChannel(Hit, StartTracee, EndTracee, ECC_Visibility);

			


			/*DrawDebugLine(GetWorld(), StartTracee, EndTracee, FColor::Blue, false, 0.5f, 0.0f, 1.0f);
			DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(2.f, 2.f, 2.f), FColor::Red, false, 0.5, ECC_Visibility, 1.f);*/
			BoxLoc = Hit.ImpactPoint;
			NormalBox= Hit.ImpactNormal;
			AStaticMeshActor* Mesh = Cast<AStaticMeshActor>(Hit.Actor);

			if (Mesh != NULL)
			{
			
				
				UKismetSystemLibrary::GetActorBounds(Mesh, Origin, BoxExtent);

			
				if (BoxExtent.Z > 130)
				{



				auto messagee = FString::Printf(TEXT("%f"), BoxLoc.Y);

					GEngine->AddOnScreenDebugMessage(-1, 3.1f, FColor::Orange, messagee);
					//En
					//BoxExtent.Y;

					////Derinlik
					//BoxExtent.X;

				

				if (BoxLoc.Y <(Origin.Y - (BoxExtent.Y / 2)))
				{
				FVector FZ(0, 0, 90);
				FVector Final = (NormalBox * 10) + BoxLoc + FZ;
			    FNavLocation NavLoc = FNavLocation(Final);
				WallCoverPoint.Add(NavLoc);
				}

				//else if (BoxLoc.Y > BoxExtent.Y - 80)
				//{
				//	FVector FZ(0, 0, 90);
				//	FVector Final(Origin.Y);
				//	FNavLocation NavLoc = FNavLocation(Final);
				//	WallCoverPoint.Add(NavLoc);
				//}

					//else if (BoxLoc.Y > BoxExtent.Y - 80)
					//{
					//	FVector FZ(0, 0, 90);
					//	FVector Final(Origin.Y);
					//	FNavLocation NavLoc = FNavLocation(Final);
					//	WallCoverPoint.Add(NavLoc);
					//}

					//BoxExtent;
					//	FVector Final = 
					//	FNavLocation NavLoc = FNavLocation(Final);
					//	WallCoverPoint.Add(NavLoc);

					//	GEngine->AddOnScreenDebugMessage(-1, 3.1f, FColor::Orange, TEXT("Kose ekleme basarili"));
					//

				//   FVector Final = (NormalBox * 10) + BoxLoc + FZ;

			    }

				else if (BoxExtent.Z < 130)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.1f, FColor::Orange, TEXT("Kısa Obje"));

					FVector FZ(0, 0, 90);
					FVector Final = (NormalBox * 10) + BoxLoc + FZ;
					FNavLocation NavLoc = FNavLocation(Final);
					ItemCandidates.Add(NavLoc);
					GAI->PointLocation(NavLoc);
				}

			}
			}


			//Noktaları gösterir, geçersizleri siler
			ProjectAndFilterNavPoints(ItemCandidates, QueryInstance);
			ProjectAndFilterNavPoints(WallCoverPoint, QueryInstance);



			//Sonuçları Sakla :)
			StoreNavPoints(ItemCandidates, QueryInstance);
			StoreNavPoints(WallCoverPoint, QueryInstance);

			
		}
}
