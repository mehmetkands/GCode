#include "ScoreBoard.h"
#include "ScoreTabWidget.h"
#include "GGPlayerState.h"
#include "GeleceginGunluguCharacter.h"


UScoreBoard::UScoreBoard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UScoreBoard::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<AGeleceginGunluguCharacter*> PlayersCharacters;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGeleceginGunluguCharacter::StaticClass(), FoundActors);

	for (AActor* index : FoundActors)
	{
		if (index)
		{
			PlayersCharacters.Add(Cast<AGeleceginGunluguCharacter>(index));
		}

		for (i = 0; i <= 4; i++)
		{
			Res = i;
		}
	}

	for (AGeleceginGunluguCharacter* GACT : PlayersCharacters)
	{
		if (GACT)
		{


			UUserWidget* TheWidget = CreateWidget(this, Scorewidgetcomponent);
			UScoreTabWidget* Widget = Cast<UScoreTabWidget>(TheWidget);
			AGeleceginGunluguCharacter* GGC = Cast<AGeleceginGunluguCharacter>(GACT);
			AGGPlayerState* Ggp= Cast<AGGPlayerState>(GACT->GetPlayerState());
			Widget->ShowNumberWidget(Res);
			Widget->ShowScoreWidget(Ggp->GetPlayerScore());
			Widget->ShowNameWidget(Ggp->GetPlayerName());

			ScoreBox->AddChildToVerticalBox(TheWidget);
		}
	}
}
