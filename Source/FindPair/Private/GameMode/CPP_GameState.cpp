// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/GameMode/CPP_GameState.h"

#ifndef CPP_STATICLIBRARY_H
#define CPP_STATICLIBRARY_H
#include "FindPair/Classes/CPP_StaticLibrary.h"
#endif

ACPP_GameState::ACPP_GameState() : CardsDelegatesRef(nullptr),
                                   SpawnCardsManager(nullptr),
                                   MI_GeneralCardBack(nullptr),
                                   HalloweenCardsDataTable(nullptr),
                                   AutumnCardsDataTable(nullptr),
                                   WinterCardsDataTable(nullptr),
                                   BakeryCardsDataTable(nullptr),
                                   SportCardsDataTable(nullptr),
                                   CleaningCardsDataTable(nullptr),
                                   CovidCardsDataTable(nullptr),
                                   FlowersCardsDataTable(nullptr),
                                   SpringCardsDataTable(nullptr),
                                   SummerCardsDataTable(nullptr),
                                   ValentinesCardsDataTable(nullptr),
                                   SchoolCardsDataTable(nullptr),
                                   SkyMaterialsDataTable(nullptr),
                                   bUserOpenedFirstCard(false),
                                   FirstViewedCard(nullptr),
                                   bUserOpenedSecondCard(false),
                                   SecondViewedCard(nullptr),
                                   AllPairsNumber(1),
                                   FoundPairsNumber(0),
                                   ClicksNumber(0),
                                   CurrentLevelMode(ELevelMode::Classic)
{
}

void ACPP_GameState::BeginPlay()
{
	Super::BeginPlay();

	if (UCPP_GameInstance* GI = GetWorld()->GetGameInstance<UCPP_GameInstance>())
	{
		StartGameDelegateHandle = GI->StartGameDelegate.AddUObject(this, &ACPP_GameState::StartLevel);
	}

	if (IsValid(CardsDelegatesRef))
	{
		CardsDelegatesRef->ResetCountersBeforeRestartingLevelDelegate.BindUObject(
			this, &ACPP_GameState::ResetPropertiesToRestartLevel);
		CardsDelegatesRef->CardWasOpenedByPlayerDelegate.BindUObject(this, &ACPP_GameState::UserTriedToOpenCard);
		CardsDelegatesRef->PlayerTriesToCloseCardDelegate.BindUObject(this, &ACPP_GameState::UserTriedToCloseCard);
		CardsDelegatesRef->GameOverDelegate.BindUObject(this, &ACPP_GameState::GameOver);
	}
}

void ACPP_GameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ResetShowTwoCardsTimer();

	if (UCPP_GameInstance* GI = GetWorld()->GetGameInstance<UCPP_GameInstance>())
	{
		GI->StartGameDelegate.Remove(StartGameDelegateHandle);
	}
	StartGameDelegateHandle.Reset();

	if (IsValid(GetWorld()) && GetWorld()->GetTimerManager().TimerExists(TH_CallStartLevelDelegate))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CallStartLevelDelegate);
	}

	if (IsValid(CardsDelegatesRef))
	{
		if (CardsDelegatesRef->ResetCountersBeforeRestartingLevelDelegate.IsBound())
		{
			CardsDelegatesRef->ResetCountersBeforeRestartingLevelDelegate.Unbind();
		}
		if (CardsDelegatesRef->CardWasOpenedByPlayerDelegate.IsBound())
		{
			CardsDelegatesRef->CardWasOpenedByPlayerDelegate.Unbind();
		}
		if (CardsDelegatesRef->PlayerTriesToCloseCardDelegate.IsBound())
		{
			CardsDelegatesRef->PlayerTriesToCloseCardDelegate.Unbind();
		}
		if (CardsDelegatesRef->GameOverDelegate.IsBound())
		{
			CardsDelegatesRef->GameOverDelegate.Unbind();
		}
	}

	CardsDelegatesRef = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ACPP_GameState::StartLevel(const int32 PairsNumber,
                                const ELevelMode LevelModeToUse,
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
		TArray<TSoftClassPtr<ACPP_ParentCard>> CardsClasses;
		if (Style == ECardsStyles::Halloween)
		{
			UCPP_StaticLibrary::GetSoftReferencesToCardsFromDataTable(HalloweenCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Autumn)
		{
			UCPP_StaticLibrary::GetSoftReferencesToCardsFromDataTable(AutumnCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Winter)
		{
			UCPP_StaticLibrary::GetSoftReferencesToCardsFromDataTable(WinterCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Bakery)
		{
			UCPP_StaticLibrary::GetSoftReferencesToCardsFromDataTable(BakeryCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Sport)
		{
			UCPP_StaticLibrary::GetSoftReferencesToCardsFromDataTable(SportCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Cleaning)
		{
			UCPP_StaticLibrary::GetSoftReferencesToCardsFromDataTable(CleaningCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Covid)
		{
			UCPP_StaticLibrary::GetSoftReferencesToCardsFromDataTable(CovidCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Flowers)
		{
			UCPP_StaticLibrary::GetSoftReferencesToCardsFromDataTable(FlowersCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Spring)
		{
			UCPP_StaticLibrary::GetSoftReferencesToCardsFromDataTable(SpringCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Summer)
		{
			UCPP_StaticLibrary::GetSoftReferencesToCardsFromDataTable(SummerCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::Valentines)
		{
			UCPP_StaticLibrary::GetSoftReferencesToCardsFromDataTable(ValentinesCardsDataTable, CardsClasses);
		}
		else if (Style == ECardsStyles::School)
		{
			UCPP_StaticLibrary::GetSoftReferencesToCardsFromDataTable(SchoolCardsDataTable, CardsClasses);
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
				CallChangingSkyMaterial(RowName);
			}
		}
	}
	GetWorld()->GetTimerManager().SetTimer(
		TH_CallStartLevelDelegate,
		[&]()
		{
			if (IsValid(GetWorld()) && GetWorld()->GetTimerManager().TimerExists(TH_CallStartLevelDelegate))
			{
				GetWorld()->GetTimerManager().ClearTimer(TH_CallStartLevelDelegate);
			}
			if (CardsDelegatesRef->ShouldStartLevelDelegate.IsBound())
			{
				CardsDelegatesRef->ShouldStartLevelDelegate.Execute(CurrentLevelMode);
			}
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
			if (CardsDelegatesRef->OnePairWasFoundDelegate.IsBound())
			{
				CardsDelegatesRef->OnePairWasFoundDelegate.Execute();
			}

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

	if (IsValid(GetWorld()) && GetWorld()->GetTimerManager().TimerExists(ShowTwoCardsTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(ShowTwoCardsTimer);
	}
}

void ACPP_GameState::ResetPropertiesToRestartLevel()
{
	ResetShowTwoCardsTimer();
	FoundPairsNumber = 0;
	ClicksNumber = 0;
}

void ACPP_GameState::LevelWasEnded(const bool bGameEndedSuccessfully)
{
	float GamingTime = -1.0f;
	if (CardsDelegatesRef->GetTimeInGameAfterFindingAllPairsDelegate.IsBound())
	{
		GamingTime = CardsDelegatesRef->GetTimeInGameAfterFindingAllPairsDelegate.Execute();
	}
	const UCPP_GameInstance* GI = GetWorld()->GetGameInstance<UCPP_GameInstance>();
	if (IsValid(GI))
	{
		if (GI->LevelWasEndedDelegate.IsBound())
		{
			GI->LevelWasEndedDelegate.Broadcast(bGameEndedSuccessfully, AllPairsNumber, CurrentLevelMode,
			                                    ClicksNumber, GamingTime);
		}
	}
	ResetPropertiesToRestartLevel();
}

void ACPP_GameState::GameOver()
{
	LevelWasEnded(false);
}

void ACPP_GameState::CallChangingSkyMaterialToDefault() const
{
	CallChangingSkyMaterial(FName(TEXT("BaseSky")));
}

void ACPP_GameState::CallChangingSkyMaterial(const FName& RowName) const
{
	TSoftObjectPtr<UMaterialInstanceConstant> OutReference;
	FRotator SkyStaticMeshRotation;
	UCPP_StaticLibrary::GetSoftReferenceToSkyMaterialFromDataTableByRowName(SkyMaterialsDataTable, RowName,
	                                                                        OutReference, SkyStaticMeshRotation);

	if (!OutReference.IsNull())
	{
		ED_ChangeSkyMaterial.Broadcast(OutReference, SkyStaticMeshRotation);
	}
}
