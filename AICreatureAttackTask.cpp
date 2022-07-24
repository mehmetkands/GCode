UKillerEaterAttack::UKillerEaterAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Player Last Location and Attack!");
}

EBTNodeResult::Type UKillerEaterAttack::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// get TargetLocation from blackboard via AI controller
	TArray<float>Distance;
	auto const cont = owner_comp.GetAIOwner();
	AKillerEater* AKE = Cast<AKillerEater>(cont->GetPawn());
	AKillerEaterAIController* CharacterTarget = Cast<AKillerEaterAIController>(owner_comp.GetAIOwner());

  // Şimdilik çalışıyor ama Get Value ile çağırılmalı!
		if (bbkeys::semisee)
		{

			//FVector::Distance(Playerlocation, AILocationn) <= 800
			TArray<AActor*>TargetActor;


			if (AKE != NULL)
			{
				if (CharacterTarget)
				{
					//Dedectedpawn array çağrılır. Dizi içerisinden en yakın düşman birincil hedeftir.
					for (AActor* Act : CharacterTarget->DetectedPawns)
					{
						if (Act)
						{
							if (AGeleceginGunluguCharacter* CastGG = Cast<AGeleceginGunluguCharacter>(Act))
							{
								float Vector = FVector::Distance(CastGG->GetActorLocation(), AKE->GetActorLocation());
								Distance.Add(Vector);
								float Minumum = Distance[0];
								for (int i = 0; i < Distance.Num(); i++)
								{
									if (Distance[i] < Minumum)
									{
										Minumum = Distance[i];
									}
								}

								if (FVector::Distance(CastGG->GetActorLocation(), AKE->GetActorLocation()) == Minumum)
								{
									playerlastseenloc = CastGG->GetActorLocation();
								}
							}

							if (AGPSMultiplayerTaskObject* CastOb = Cast<AGPSMultiplayerTaskObject>(Act))
							{

								float Vector = FVector::Distance(CastOb->GetActorLocation(), AKE->GetActorLocation());

								Distance.Add(Vector);
								float Minumum = Distance[0];
								
								for (int i = 0; i < Distance.Num(); i++)
								{
									if (Distance[i] < Minumum)
									{
										Minumum = Distance[i];
									}
								}
	
								if (FVector::Distance(CastOb->GetActorLocation(), AKE->GetActorLocation()) == Minumum)
								{
									playerlastseenloc = CastOb->GetActorLocation();
								}
							}
						}
					}
				}
			}
			
			FVector(playerlastseenloc.X, playerlastseenloc.Y, 0);
}
	owner_comp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), playerlastseenloc);

	// Lokasyona temel hareket.
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, playerlastseenloc);

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	
	return EBTNodeResult::Succeeded;
}
