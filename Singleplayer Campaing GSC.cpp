ASinglePlayerCampaing1::ASinglePlayerCampaing1()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
	Multiplayer = false;
	//Default blueprint karakterinin yolu
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Weapons/Characters/Player/FpsCam"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

    //Hud Sınıfı
	static ConstructorHelpers::FClassFinder<AHUD> HudPawnClassFinder(TEXT("/Game/Weapons/GGCHUDBP"));
	HUDClass = HudPawnClassFinder.Class;

	GameStateClass = AGGGameStateBase::StaticClass();

	PlayerStateClass = AGGPlayerState::StaticClass();

	PlayerControllerClass = AGGCPlayerController::StaticClass();
}

void ASinglePlayerCampaing1::BeginPlay()
{
	Super::BeginPlay();

	TheMissionWidget = CreateWidget<UTitleAndMissionWidget>(GetWorld(), MissionWidget);
	if (TheMissionWidget != NULL)
	{
		TheMissionWidget->AddToViewport();
	}

	CurrentMission = 1;
	GasTankRemaining = 10;
	booleanCounter = 0;
	ReachtheCrashSide = false;
	SearchAlive = false;
	ReachtheHouse = false;
	TakeWeapon = false;
	ReachtheMedicals = false;
	GetBackHouse = false;
	SpeakSgt = false;
	GotoRadioTower = false;
	SearchRadioSystem = false;
	BackRadioTower = false;
	ContacttoBase = false;
	CEnd = false;

	SearchKillerEaterEggs = false;
	SearchColdSpiderEggs = false;

	TitleShower();
	GameVic = false;
}

void ASinglePlayerCampaing1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MissionController();
	MissionCheckController();

	if (MedicalObjectCounter >= 6)
	{
		CompleteMissionReachtheMedicals();
	}
}


void ASinglePlayerCampaing1::GameVictory()
{
	GameVic = true;
}

void ASinglePlayerCampaing1::GameLose()
{
}

void ASinglePlayerCampaing1::CompleteMissionReachtheCrashSide()
{
	ReachtheCrashSide = true;
	CReachtheCrashSide = true;
	CurrentMission = 2;
}

void ASinglePlayerCampaing1::CompleteMissionSearchAlive()
{
	if (CSearchAlive==false&&ReachtheCrashSide == true)
	{
		SearchAlive = true;
		CSearchAlive = true;
		CurrentMission = 3;
	}
}

void ASinglePlayerCampaing1::CompleteMissionReachtheHouse()
{
	if (CReachtheHouse==false&&SearchAlive == true)
	{
		ReachtheHouse = true;
		CReachtheHouse = true;
		CurrentMission = 4;
	}
}

//Task
void ASinglePlayerCampaing1::CompleteMissionTakeWeapon()
{
	if (CTakeWeapon==false&&ReachtheHouse == true)
	{
		TakeWeapon = true;
		CTakeWeapon = true;
		CurrentMission = 5;
	}
}

void ASinglePlayerCampaing1::CompleteMissionReachtheMedicals()
{
	if (CReachtheMedicals==false&&TakeWeapon == true)
	{
		ReachtheMedicals = true;
		CReachtheMedicals = true;
		CurrentMission = 6;
	}
}


void ASinglePlayerCampaing1::CompleteMissionGetBackHouse()
{
	if (CGetBackHouse==false&&ReachtheMedicals == true)
	{
		GetBackHouse = true;
		CGetBackHouse = true;
		CurrentMission = 7;
	}
}

void ASinglePlayerCampaing1::CompleteMissionSpeakSgt()
{
	if (CSpeakSgt==false&&GetBackHouse == true)
	{
		SpeakSgt = true;
		CSpeakSgt = true;
		CurrentMission = 8;
	}
}

void ASinglePlayerCampaing1::CompleteMissionGotoRadioTower()
{
	if (CGotoRadioTower==false&&SpeakSgt == true)
	{
		GotoRadioTower = true;
		CGotoRadioTower = true;
		CurrentMission = 9;
	}
}

void ASinglePlayerCampaing1::CompleteMissionSearchRadioSystem()
{
	if (CSearchRadioSystem==false&&GotoRadioTower == true)
	{
		SearchRadioSystem = true;
		CSearchRadioSystem = true;
		CurrentMission = 10;
	}
}

void ASinglePlayerCampaing1::CompleteMissionBackRadioTower()
{
	if (CBackRadioTower==false&&SearchRadioSystem == true)
	{
		BackRadioTower = true;
		CBackRadioTower = true;
		CurrentMission = 11;
	}
}

void ASinglePlayerCampaing1::CompleteMissionContacttoBase()
{
	if (CContacttoBase==false&&BackRadioTower == true)
	{
		ContacttoBase = true;
		CContacttoBase = true;
		CurrentMission = 12;
	}
}

void ASinglePlayerCampaing1::EndGamePlay()
{
	if (CEnd == false && CContacttoBase == true)
	{
		EndM = true;
		CEnd = true;
		CurrentMission = 13;
	}
}



void ASinglePlayerCampaing1::MissionController()
{
	if (ReachtheCrashSide == true && SearchAlive == true && ReachtheHouse == true && TakeWeapon == true && 
		ReachtheMedicals == true && GetBackHouse == true && SpeakSgt == true && GotoRadioTower == true 
		&& SearchRadioSystem == true && BackRadioTower == true && ContacttoBase == true&& EndM == true)
	{
		GameVictory();
	}
}
	

void ASinglePlayerCampaing1::MissionCheckController()
{
	if (CurrentMission==1)
	{

		TheMissionWidget->ShowMissionName("Kaza Alanindan Cik");
	}

	if (CurrentMission == 2)
	{

		TheMissionWidget->ShowMissionName("Yasayan Birilerini Bul");
	}

	if (CurrentMission == 3)
	{

		TheMissionWidget->ShowMissionName("Cavusla Konus");
	}

	if (CurrentMission == 4)
	{

		TheMissionWidget->ShowMissionName("Catidan Ekipman Al");
	}

	if (CurrentMission == 5)
	{

		TheMissionWidget->ShowMissionName("Etrafta Medikal Malzeme Ara");
	}

	if (CurrentMission == 6)
	{

		TheMissionWidget->ShowMissionName("Bulduklarini Geri Gotur");
	}

	if (CurrentMission == 7)
	{

		TheMissionWidget->ShowMissionName("Cavusla Konus");
	}

	if (CurrentMission == 8)
	{

		TheMissionWidget->ShowMissionName("Radyo Kulesine Git");
	}

	if (CurrentMission == 9)
	{

		TheMissionWidget->ShowMissionName("Sistemi Kurmak İcin Cevrede Telsiz Sistemi ve Kablo Ara");
	}


	if (CurrentMission == 10)
	{

		TheMissionWidget->ShowMissionName("Radyo Kulesine Geri Don");
	}


	if (CurrentMission == 11)
	{

		TheMissionWidget->ShowMissionName("Iletisim Kur");
	}


	if (CurrentMission == 12)
	{

		TheMissionWidget->ShowMissionName("Kuzeye Dogru İlerle");
	}


	if (CurrentMission == 13)
	{

		TheMissionWidget->ShowMissionName("");
	}
}

void ASinglePlayerCampaing1::TitleShower()
{
	TheMissionWidget->ShowDate("16.02.1942");
	TheMissionWidget->ShowTitle("Cold Sun");
	TheMissionWidget->ShowName("Sgt.Kurt");
	TheMissionWidget->ShowForceName("Penial Cargo Guard");
	TheMissionWidget->ShowTime("05:02");
	TheMissionWidget->ShowRegion("Ardennes");
}
