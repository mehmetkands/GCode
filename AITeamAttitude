AGGCAIController::AGGCAIController(FObjectInitializer const& object_initializer)
{
	SetGenericTeamId(FGenericTeamId(TeamNumber));

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Weapons/AI/GGAIBTree.GGAIBTree'"));
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


	//()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AGGCAIController::PawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->ConfigureSense(*HearingConfig);

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = true;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	 
	HearingConfig->HearingRange = 3000.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	bNeedCover = true;
	bTakeCoverDone = false;
}

ETeamAttitude::Type AGGCAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const AGeleceginGunluguCharacter* AnotherPawnT = Cast<AGeleceginGunluguCharacter>(&Other);
	if (AnotherPawnT)
	{
		if (const IGenericTeamAgentInterface* TeamInfo = Cast<IGenericTeamAgentInterface>(AnotherPawnT->GetController()))
		{


			FGenericTeamId TeamIDNumber = TeamInfo->GetGenericTeamId();

			if (TeamIDNumber == TeamNumber)
			{
				return ETeamAttitude::Friendly;
			}

			else
			{
				return ETeamAttitude::Hostile;
			}
		}

		const IGenericTeamAgentInterface* PlayerTeamInfo = Cast<IGenericTeamAgentInterface>(&Other);
		if (PlayerTeamInfo)
		{

			FGenericTeamId PlayerTeamIDNumber = PlayerTeamInfo->GetGenericTeamId();

			if (PlayerTeamIDNumber == TeamNumber)
			{

				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;

			}
		}
	}
  
	return ETeamAttitude::Neutral;

	const ABlackforceCharacter* BTAnotherPawn = Cast<ABlackforceCharacter>(&Other);
	if (BTAnotherPawn)
	{
		if (const IGenericTeamAgentInterface* TeamInfo = Cast<IGenericTeamAgentInterface>(BTAnotherPawn->GetController()))
		{
			FGenericTeamId TeamIDNumber = TeamInfo->GetGenericTeamId();

			if (TeamIDNumber == TeamNumber)
			{
				return ETeamAttitude::Friendly;
			}

			else
			{
				return ETeamAttitude::Hostile;
			}
		}

		const IGenericTeamAgentInterface* PlayerTeamInfo = Cast<IGenericTeamAgentInterface>(&Other);
		if (PlayerTeamInfo)
		{
		
			FGenericTeamId PlayerTeamIDNumber = PlayerTeamInfo->GetGenericTeamId();

			if (PlayerTeamIDNumber == TeamNumber)
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;
			}
		}

	}
	return ETeamAttitude::Neutral;
}

void AGGCAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BTree);
	BTComponent->StartTree(*BTree);
	SetTimerButton = false;
}

void AGGCAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);

	if (BlackBoard)
	{
		BlackBoard->InitializeBlackboard(*BTree->BlackboardAsset);
	}
}

UBlackboardComponent* AGGCAIController::GetBlackboard() const
{
	return BlackBoard;
}


void AGGCAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
