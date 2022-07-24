// Fill out your copyright notice in the Description page of Project Settings.


#include "KillerEaterAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "GeleceginGunluguCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GPSMultiplayerTaskObject.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "blackboard_keys.h"
#include "ai_tags.h"
#include "TimerManager.h" 

AKillerEaterAIController::AKillerEaterAIController(FObjectInitializer const& object_initializer)
{
  	//Davranış Ağacının Dizini
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Weapons/AI/KillerAIBTree.KillerAIBTree'"));
	
	//Davranış Ağacının Dizini Başarılıysa
	if (obj.Succeeded())
	{
		BTree = obj.Object;
	}

	BTComponent = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackBoard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackBoardComp"));


	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sense Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));


	//Algı Güncellemesi
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AKillerEaterAIController::PawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	//Görme ve Algılar
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = true;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	//İşitme Değerleri
	HearingConfig->HearingRange = 3000.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
}

void AKillerEaterAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BTree);
	BTComponent->StartTree(*BTree);
}

void AKillerEaterAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);

	if (BlackBoard)
	{
		BlackBoard->InitializeBlackboard(*BTree->BlackboardAsset);
	}
}

UBlackboardComponent* AKillerEaterAIController::GetBlackboard() const
{
	return BlackBoard;
}


void AKillerEaterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	
}

void AKillerEaterAIController::PawnDetected(AActor* actor, FAIStimulus const stimulus)
{
	//Algılanan sınıf oyuncu karakteriyse
	if (auto const ch = Cast<AGeleceginGunluguCharacter>(actor))
	{
	
		GetBlackboard()->SetValueAsBool(bbkeys::DemonCanSeePlayer, stimulus.WasSuccessfullySensed());


		if (bbkeys::semisee, stimulus.WasSuccessfullySensed())
		{
			perception = 2.0f;
			GetWorldTimerManager().SetTimer(SureSight, this, &AKillerEaterAIController::TMR, 2.0f, true);

			if (ch->HealthOverride > 0)
			{
				//0'dan büyük ise dedectedpawn array'e eklenir. Böylece algılanan tüm düşmanlar arrayde yer bulur. 
				//Array listesi Task sırasında kullanılır.
				DetectedPawns.Add(ch);

			}

		}

		else
		{
			perception = 1.0f;
			if (perception == 1.0f)
			{
				GetBlackboard()->SetValueAsBool(bbkeys::canseeplayer, false);
				DetectedPawns.Remove(ch);
			}
		}
	}

	//Eğer saldırılacak nesne bir obje ise(Multiplayer için)
	if (auto const Object = Cast<AGPSMultiplayerTaskObject>(actor))
	{

		GetBlackboard()->SetValueAsBool(bbkeys::DemonCanSeePlayer, stimulus.WasSuccessfullySensed());


		if (bbkeys::semisee, stimulus.WasSuccessfullySensed())
		{
			perception = 2.0f;
			GetWorldTimerManager().SetTimer(SureSight, this, &AKillerEaterAIController::TMR, 2.0f, true);

			if (Object->ObjectHealth > 0)
			{
				DetectedPawns.Add(Object);
			}

		}

		else
		{
			perception = 1.0f;

			if (perception == 1.0f)
			{
				GetBlackboard()->SetValueAsBool(bbkeys::canseeplayer, false);
				DetectedPawns.Remove(Object);
			}
		}
	}
}



void AKillerEaterAIController::TMR()
{
	if (perception == 2.0f)
	{
		GetBlackboard()->SetValueAsBool(bbkeys::canseeplayer, true);
	}


}

//Self Aktör Bulucu
void AKillerEaterAIController::ActorSelfFounder(AActor* Actor)
{
	SelfActor = Actor;
}

//Zekayı sıfırlar. Ölüm fonksiyonunda çağrılır.
void AKillerEaterAIController::SysStopLogic()
{
	GetBrainComponent()->StopLogic("Dead");
}
