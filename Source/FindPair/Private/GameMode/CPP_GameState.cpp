﻿// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/GameMode/CPP_GameState.h"

ACPP_GameState::ACPP_GameState()
{
	CardsDelegatesRef = nullptr;
	SpawnCardsManager = nullptr;
	MI_GeneralCardBack = nullptr;

	HalloweenCardsDataTable = nullptr;
	AutumnCardsDataTable = nullptr;
	WinterCardsDataTable = nullptr;
	BakeryCardsDataTable = nullptr;
	SportCardsDataTable = nullptr;
	CleaningCardsDataTable = nullptr;
	CovidCardsDataTable = nullptr;
	FlowersCardsDataTable = nullptr;
	SpringCardsDataTable = nullptr;
	SummerCardsDataTable = nullptr;
	ValentinesCardsDataTable = nullptr;
	SchoolCardsDataTable = nullptr;
	SkyMaterialsDataTable = nullptr;

	FirstViewedCard = nullptr;
	SecondViewedCard = nullptr;
	bUserOpenedFirstCard = false;
	bUserOpenedSecondCard = false;
	CurrentLevelMode = ELevelMode::Classic;
	AllPairsNumber = 1;
	FoundPairsNumber = 0;
	ClicksNumber = 0;
}

void ACPP_GameState::BeginPlay()
{
	Super::BeginPlay();

	if (UCPP_GameInstance* GI = GetWorld()->GetGameInstance<UCPP_GameInstance>())
	{
		StartGameDelegateHandle = GI->StartGameDelegate.AddUObject(this, &ACPP_GameState::StartLevel);
	}

	CardsDelegatesRef->CardWasOpenedByPlayerDelegate.BindUObject(this, &ACPP_GameState::UserTriedToOpenCard);
	CardsDelegatesRef->PlayerTriesToCloseCardDelegate.BindUObject(this, &ACPP_GameState::UserTriedToCloseCard);
	CardsDelegatesRef->GameOverDelegate.BindUObject(this, &ACPP_GameState::GameOver);
}

void ACPP_GameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ResetShowTwoCardsTimer();

	if (UCPP_GameInstance* GI = GetWorld()->GetGameInstance<UCPP_GameInstance>())
	{
		GI->StartGameDelegate.Remove(StartGameDelegateHandle);
	}
	StartGameDelegateHandle.Reset();

	CardsDelegatesRef = nullptr;
	SpawnCardsManager = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ACPP_GameState::StartLevel(int32& PairsNumber,
                                ELevelMode& LevelModeToUse,
                                TArray<ECardsStyles>& CardsStylesToUse)
{
	if (!IsValid(SpawnCardsManager))
		return;
	
	ResetShowTwoCardsTimer();
	CurrentLevelMode = LevelModeToUse;
	AllPairsNumber = PairsNumber;
	FoundPairsNumber = 0;
	ClicksNumber = 0;
	
	SpawnCardsManager->AllCardsClasses.Empty();
	SpawnCardsManager->NumberOfPairs = PairsNumber;

	for (const auto& Style : CardsStylesToUse)
	{
		TArray<TAssetSubclassOf<ACPP_ParentCard>> CardsClasses;
		if (Style == ECardsStyles::Halloween)
		{
			GetSoftReferencesToCardsFromDataTable(HalloweenCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Autumn)
		{
			GetSoftReferencesToCardsFromDataTable(AutumnCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Winter)
		{
			GetSoftReferencesToCardsFromDataTable(WinterCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Bakery)
		{
			GetSoftReferencesToCardsFromDataTable(BakeryCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Sport)
		{
			GetSoftReferencesToCardsFromDataTable(SportCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Cleaning)
		{
			GetSoftReferencesToCardsFromDataTable(CleaningCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Covid)
		{
			GetSoftReferencesToCardsFromDataTable(CovidCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Flowers)
		{
			GetSoftReferencesToCardsFromDataTable(FlowersCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Spring)
		{
			GetSoftReferencesToCardsFromDataTable(SpringCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Summer)
		{
			GetSoftReferencesToCardsFromDataTable(SummerCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Valentines)
		{
			GetSoftReferencesToCardsFromDataTable(ValentinesCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::School)
		{
			GetSoftReferencesToCardsFromDataTable(SchoolCardsDataTable, CardsClasses);
		}
		SpawnCardsManager->AllCardsClasses.Append(CardsClasses);
	}

	if (CardsStylesToUse.Num() > 1)
	{
		SpawnCardsManager->bSeveralCardsStylesAreUsed = true;
		SpawnCardsManager->MI_GeneralCardBack = MI_GeneralCardBack;
	}
	else
	{
		SpawnCardsManager->bSeveralCardsStylesAreUsed = false;
		if (CardsStylesToUse.Num() == 1)
		{
			FName RowName = FName(NAME_None);

			if (CardsStylesToUse[0] == ECardsStyles::Winter)
			{
				RowName = FName(TEXT("WinterSky"));
			}
			else if (CardsStylesToUse[0] == ECardsStyles::Autumn ||
				CardsStylesToUse[0] == ECardsStyles::Valentines ||
				CardsStylesToUse[0] == ECardsStyles::School ||
				CardsStylesToUse[0] == ECardsStyles::Covid ||
				CardsStylesToUse[0] == ECardsStyles::Flowers)
			{
				RowName = FName(TEXT("DarkSky1"));
			}
			else if (CardsStylesToUse[0] == ECardsStyles::Summer ||
				CardsStylesToUse[0] == ECardsStyles::Cleaning)
			{
				RowName = FName(TEXT("DarkSky2"));
			}

			if (!RowName.IsNone())
			{
				TAssetPtr<UMaterialInstanceConstant> OutReference;
				FRotator SkyStaticMeshRotation;
				GetSoftReferenceToSkyMaterialFromDataTableByRowName(SkyMaterialsDataTable,
				                                                    RowName,
				                                                    OutReference,
				                                                    SkyStaticMeshRotation);
				if (!OutReference.IsNull())
				{
					ED_ChangeSkyMaterial.Broadcast(OutReference, SkyStaticMeshRotation);
				}
			}
		}
	}
	FTimerHandle TmpTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TmpTimerHandle,
		[&]()
		{
			CardsDelegatesRef->ShouldStartLevelDelegate.ExecuteIfBound(CurrentLevelMode);
		},
		0.1f,
		false);
}

void ACPP_GameState::UserTriedToOpenCard(ACPP_ParentCard* CardRef)
{
	if (GetWorld()->GetTimerManager().TimerExists(ShowTwoCardsTimer))
		return;

	ClicksNumber++;

	if (!bUserOpenedFirstCard)
	{
		bUserOpenedFirstCard = true;
		FirstViewedCard = CardRef;
		FirstViewedCard->OpenCard();
	}
	else if (!bUserOpenedSecondCard)
	{
		if (CardRef->GetCardId() == FirstViewedCard->GetCardId())
		{
			CardsDelegatesRef->OnePairWasFoundDelegate.ExecuteIfBound();

			CardRef->OpenCard();
			CardRef->CardWasFound();
			FirstViewedCard->CardWasFound();
			bUserOpenedFirstCard = false;
			FirstViewedCard = nullptr;
			FoundPairsNumber++;

			if (FoundPairsNumber == AllPairsNumber)
			{
				LevelWasEnded(true);
			}
		}
		else
		{
			bUserOpenedSecondCard = true;
			SecondViewedCard = CardRef;
			SecondViewedCard->OpenCard();
			GetWorld()->GetTimerManager().SetTimer(ShowTwoCardsTimer,
			                                       this,
			                                       &ACPP_GameState::ResetShowTwoCardsTimer,
			                                       0.57f,
			                                       false);
		}
	}
}

void ACPP_GameState::UserTriedToCloseCard(ACPP_ParentCard* CardRef)
{
	if (bUserOpenedFirstCard && bUserOpenedSecondCard)
		return;

	if (bUserOpenedFirstCard && FirstViewedCard == CardRef)
	{
		bUserOpenedFirstCard = false;
		FirstViewedCard->CloseCard();
		FirstViewedCard = nullptr;
	}
}

void ACPP_GameState::ResetShowTwoCardsTimer()
{
	if (IsValid(FirstViewedCard))
	{
		FirstViewedCard->CloseCard();
	}
	FirstViewedCard = nullptr;
	bUserOpenedFirstCard = false;

	if (IsValid(SecondViewedCard))
	{
		SecondViewedCard->CloseCard();
	}
	SecondViewedCard = nullptr;
	bUserOpenedSecondCard = false;

	if (GetWorld()->GetTimerManager().TimerExists(ShowTwoCardsTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(ShowTwoCardsTimer);
	}
}

void ACPP_GameState::LevelWasEnded(bool bGameEndedSuccessfully)
{
	float GamingTime = -1.0f;
	if (CardsDelegatesRef->GetTimeInGameAfterFindingAllPairsDelegate.IsBound())
	{
		GamingTime = CardsDelegatesRef->GetTimeInGameAfterFindingAllPairsDelegate.Execute();
	}
	UCPP_GameInstance* GI = GetWorld()->GetGameInstance<UCPP_GameInstance>();
	if (IsValid(GI))
	{
		if (GI->LevelWasEndedDelegate.IsBound())
		{
			GI->LevelWasEndedDelegate.Broadcast(bGameEndedSuccessfully, AllPairsNumber, CurrentLevelMode,
			                                    ClicksNumber, GamingTime);
		}
	}
	FoundPairsNumber = 0;
	ClicksNumber = 0;
}

void ACPP_GameState::GameOver()
{
	LevelWasEnded(false);
}

void ACPP_GameState::CallChangingSkyMaterialToDefault()
{
	TAssetPtr<UMaterialInstanceConstant> OutReference;
	FRotator SkyStaticMeshRotation;
	GetSoftReferenceToSkyMaterialFromDataTableByRowName(SkyMaterialsDataTable, FName(TEXT("BaseSky")),
	                                                    OutReference, SkyStaticMeshRotation);

	if (!OutReference.IsNull())
	{
		ED_ChangeSkyMaterial.Broadcast(OutReference, SkyStaticMeshRotation);
	}
}

void ACPP_GameState::GetSoftReferenceToSkyMaterialFromDataTableByRowName(UDataTable*& DataTable, FName RowName,
                                                                         TAssetPtr<UMaterialInstanceConstant>&
                                                                         OutReference,
                                                                         FRotator& OutSkyStaticMeshRotation)
{
	const FSkyMaterialTableRow* FoundRow = DataTable->FindRow<FSkyMaterialTableRow>(RowName,
		TEXT(""),
		true);

	OutReference = FoundRow->SkyMaterialClass;
	OutSkyStaticMeshRotation = FoundRow->RelatedSkySphereRotation;
}

void ACPP_GameState::GetSoftReferencesToCardsFromDataTable(UDataTable*& DataTable,
                                                           TArray<TAssetSubclassOf<ACPP_ParentCard>>& OutReferences)
{
	TArray<FName> RowNames = DataTable->GetRowNames();
	for (const auto& Name : RowNames)
	{
		FCardTableRow* Item = DataTable->FindRow<FCardTableRow>(Name, TEXT(""));
		OutReferences.Emplace(Item->CardClass);
	}
}
