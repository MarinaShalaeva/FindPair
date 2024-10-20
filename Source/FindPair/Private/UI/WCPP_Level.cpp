// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/UI/WCPP_Level.h"

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "FindPair/Classes/UI/CPP_StaticWidgetLibrary.h"
#endif

#ifndef CPP_GAMEINSTANCE_H
#define CPP_GAMEINSTANCE_H
#include "FindPair/Classes/GameMode/CPP_GameInstance.h"
#endif
class UCPP_GameInstance;

#include "Kismet/KismetMathLibrary.h"

UWCPP_Level::UWCPP_Level(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                        PairsNumberHorizontalBox(nullptr),
                                                                        FoundPairsNumberTextBlock(nullptr),
                                                                        AllPairsNumberTextBlock(nullptr),
                                                                        AddSecondsTextBlock(nullptr),
                                                                        StartEarlierVerticalBox(nullptr),
                                                                        SecondsBeforeStartTextBlock(nullptr),
                                                                        StartGameEarlierButton(nullptr),
                                                                        HoursTextBlock(nullptr),
                                                                        ColonBetweenHoursAndMinutes(nullptr),
                                                                        MinutesTextBlock(nullptr),
                                                                        SecondsTextBlock(nullptr),
                                                                        PauseButton(nullptr),
                                                                        CardsDelegatesRef(nullptr),
                                                                        PreviewTime(0.0f),
                                                                        SecondsInGame(0.0f),
                                                                        SecondsLeft(0.0f),
                                                                        SecondsToAdd(2.0f),
                                                                        bShouldPrintHours(false),
                                                                        FoundPairsNumber(0),
                                                                        AllPairsNumber(1),
                                                                        CurrentLevelMode(ELevelMode::Classic)
{
}

void UWCPP_Level::InitializeCardsDelegatesVariable(UCPP_CardsDelegates*& NewCardsDelegatesRef)
{
	CardsDelegatesRef = NewCardsDelegatesRef;
	if (IsValid(CardsDelegatesRef))
	{
		CardsDelegatesRef->CardsSpawningEndedDelegate.BindUObject(this, &UWCPP_Level::StartNewGame);
		CardsDelegatesRef->OnePairWasFoundDelegate.BindUObject(this, &UWCPP_Level::IncrementFoundPairsNumber);
		CardsDelegatesRef->GetTimeInGameAfterFindingAllPairsDelegate.BindUObject(this, &UWCPP_Level::ResetGamingTimers);
	}
}

void UWCPP_Level::NativeConstruct()
{
	Super::NativeConstruct();

	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(StartGameEarlierButton, false);
	UCPP_StaticWidgetLibrary::ChangePanelWidgetVisibility(StartEarlierVerticalBox,
	                                                      ESlateVisibility::Collapsed);
	UCPP_StaticWidgetLibrary::ChangePanelWidgetVisibility(PairsNumberHorizontalBox,
	                                                      ESlateVisibility::Collapsed);

	StartGameEarlierButton->OnClicked.AddUniqueDynamic(this, &UWCPP_Level::SkipPreviewPeriod);
}

void UWCPP_Level::NativeDestruct()
{
	if (IsValid(StartGameEarlierButton) && StartGameEarlierButton->OnClicked.IsAlreadyBound(this,
		&UWCPP_Level::SkipPreviewPeriod))
	{
		StartGameEarlierButton->OnClicked.RemoveDynamic(this, &UWCPP_Level::SkipPreviewPeriod);
	}

	if (IsValid(CardsDelegatesRef))
	{
		CardsDelegatesRef->CardsSpawningEndedDelegate.Unbind();
		CardsDelegatesRef->OnePairWasFoundDelegate.Unbind();
		CardsDelegatesRef->GetTimeInGameAfterFindingAllPairsDelegate.Unbind();

		CardsDelegatesRef = nullptr;
	}

	Super::NativeDestruct();
}

void UWCPP_Level::StartNewGame(int32 AllPairsNum, float LocalPreviewTime, ELevelMode LevelMode)
{
	CurrentLevelMode = LevelMode;
	ResetGamingTimers();

	SetAllPairsNumber(AllPairsNum);
	int32 FoundPairsNum = 0;
	SetFoundPairsNumber(FoundPairsNum);
	UCPP_StaticWidgetLibrary::ChangePanelWidgetVisibility(PairsNumberHorizontalBox,
	                                                      ESlateVisibility::SelfHitTestInvisible);

	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(StartGameEarlierButton, true);
	UCPP_StaticWidgetLibrary::ChangePanelWidgetVisibility(StartEarlierVerticalBox,
	                                                      ESlateVisibility::SelfHitTestInvisible);

	bShouldPrintHours = false;
	if (HoursTextBlock->GetVisibility() != ESlateVisibility::Hidden)
	{
		UCPP_StaticWidgetLibrary::ChangeTextBlocksVisibility(HoursTextBlock,
		                                                     ESlateVisibility::Hidden);
		HoursTextBlock->SetText(FText::AsCultureInvariant(TEXT("0")));
	}
	if (ColonBetweenHoursAndMinutes->GetVisibility() != ESlateVisibility::Hidden)
	{
		UCPP_StaticWidgetLibrary::ChangeTextBlocksVisibility(ColonBetweenHoursAndMinutes,
		                                                     ESlateVisibility::Hidden);
	}

	if (CurrentLevelMode == ELevelMode::Classic)
	{
		StartNewGameForClassicMode();
	}
	else if (CurrentLevelMode == ELevelMode::TimeReduction)
	{
		StartNewGameForTimeReductionMode();
	}

	PreviewTime = LocalPreviewTime;

	GetWorld()->GetTimerManager().SetTimer(LevelCanBeShownTimer,
	                                       this,
	                                       &UWCPP_Level::LevelCanBeShownToUserNotification,
	                                       1.0f,
	                                       false);
}

void UWCPP_Level::LevelCanBeShownToUserNotification()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(LevelCanBeShownTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(LevelCanBeShownTimer);
	}

	if (const UCPP_GameInstance* GI = GetWorld()->GetGameInstance<UCPP_GameInstance>())
	{
		GI->CanShowGeneratedLevelDelegate.Broadcast();
	}

	GetWorld()->GetTimerManager().SetTimer(PreviewTimer,
	                                       this,
	                                       &UWCPP_Level::UpdatePreviewTimer,
	                                       0.01f,
	                                       true);
}

void UWCPP_Level::StartNewGameForClassicMode()
{
	MinutesTextBlock->SetText(FText::AsCultureInvariant(TEXT("00")));
	SecondsTextBlock->SetText(FText::AsCultureInvariant(TEXT("00")));
}

void UWCPP_Level::StartNewGameForTimeReductionMode()
{
	if (AllPairsNumber <= 13)
	{
		SecondsLeft = AllPairsNumber * 2;
		SecondsToAdd = 2.0f;
		AddSecondsTextBlock->SetText(FText::FromString(TEXT("+2")));
	}
	else if (AllPairsNumber <= 20)
	{
		SecondsLeft = AllPairsNumber * 2.5;
		SecondsToAdd = 3.0f;
		AddSecondsTextBlock->SetText(FText::FromString(TEXT("+3")));
	}
	else if (AllPairsNumber <= 25)
	{
		SecondsLeft = AllPairsNumber * 3;
		SecondsToAdd = 4.0f;
		AddSecondsTextBlock->SetText(FText::FromString(TEXT("+4")));
	}
	else if (AllPairsNumber <= 30)
	{
		SecondsLeft = AllPairsNumber * 3.5;
		SecondsToAdd = 5.0f;
		AddSecondsTextBlock->SetText(FText::FromString(TEXT("+5")));
	}
	else if (AllPairsNumber <= 40)
	{
		SecondsLeft = AllPairsNumber * 4;
		SecondsToAdd = 6.0f;
		AddSecondsTextBlock->SetText(FText::FromString(TEXT("+6")));
	}
	else if (AllPairsNumber <= 45)
	{
		SecondsLeft = AllPairsNumber * 4.5;
		SecondsToAdd = 7.0f;
		AddSecondsTextBlock->SetText(FText::FromString(TEXT("+7")));
	}
	else
	{
		SecondsLeft = AllPairsNumber * 5;
		SecondsToAdd = 8.0f;
		AddSecondsTextBlock->SetText(FText::FromString(TEXT("+8")));
	}
	SecondsTextBlock->SetText(UCPP_StaticLibrary::GetSecondsToDisplayFromSecondsNumber(SecondsLeft));
	MinutesTextBlock->SetText(UCPP_StaticLibrary::GetMinutesToDisplayFromSecondsNumber(SecondsLeft));
}

void UWCPP_Level::UpdatePreviewTimer()
{
	const FText Result = UCPP_StaticLibrary::GetFloatAsTextWithPrecision(PreviewTime, 2, true);

	SecondsBeforeStartTextBlock->SetText(Result);

	PreviewTime -= 0.01f;
	if (PreviewTime <= 0.0f)
	{
		FinishPreviewPeriod();
	}
}

void UWCPP_Level::FinishPreviewPeriod()
{
	if (GetWorld()->GetTimerManager().TimerExists(PreviewTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(PreviewTimer);
	}

	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(StartGameEarlierButton, false);
	UCPP_StaticWidgetLibrary::ChangePanelWidgetVisibility(StartEarlierVerticalBox,
	                                                      ESlateVisibility::Collapsed);

	ActivateGamingTimer();
}

void UWCPP_Level::SkipPreviewPeriod()
{
	FinishPreviewPeriod();
	CardsDelegatesRef->SkipCardsPreviewDelegate.ExecuteIfBound();
}

void UWCPP_Level::ActivateGamingTimer()
{
	SecondsInGame = 0.0f;

	if (CurrentLevelMode == ELevelMode::Classic)
	{
		ActivateGamingTimerForClassicMode();
	}
	else if (CurrentLevelMode == ELevelMode::TimeReduction)
	{
		ActivateGamingTimerForTimeReductionMode();
	}
}

void UWCPP_Level::ActivateGamingTimerForClassicMode()
{
	GetWorld()->GetTimerManager().SetTimer(MillisecondsGamingTimer,
	                                       this,
	                                       &UWCPP_Level::UpdateMillisecondsGamingTimerForClassicMode,
	                                       0.01f,
	                                       true);

	GetWorld()->GetTimerManager().SetTimer(SecondsGamingTimer,
	                                       this,
	                                       &UWCPP_Level::UpdateTimerTextBlocksForClassicMode,
	                                       1.0f,
	                                       true,
	                                       1.0f - SecondsInGame);
}

void UWCPP_Level::UpdateMillisecondsGamingTimerForClassicMode()
{
	SecondsInGame += 0.01f;
}

void UWCPP_Level::UpdateTimerTextBlocksForClassicMode()
{
	SecondsTextBlock->SetText(UCPP_StaticLibrary::GetSecondsToDisplayFromSecondsNumber(SecondsInGame));

	const int32 RoundSeconds = UKismetMathLibrary::Round(SecondsInGame);
	if (RoundSeconds % 60 == 0)
	{
		MinutesTextBlock->SetText(UCPP_StaticLibrary::GetMinutesToDisplayFromRoundSecondsNumber(RoundSeconds));
	}
	if (RoundSeconds >= 3600)
	{
		UpdateHoursTextBlock(RoundSeconds);
	}
}

void UWCPP_Level::UpdateHoursTextBlock(int32 RoundSeconds)
{
	const int32 Hours = RoundSeconds / 3600;
	if (Hours > 0)
	{
		if (!bShouldPrintHours)
		{
			bShouldPrintHours = true;
			UCPP_StaticWidgetLibrary::ChangeTextBlocksVisibility(HoursTextBlock,
			                                                     ESlateVisibility::SelfHitTestInvisible);
			UCPP_StaticWidgetLibrary::ChangeTextBlocksVisibility(ColonBetweenHoursAndMinutes,
			                                                     ESlateVisibility::SelfHitTestInvisible);
		}
		HoursTextBlock->SetText(FText::AsNumber(Hours));
	}
}

void UWCPP_Level::ActivateGamingTimerForTimeReductionMode()
{
	GetWorld()->GetTimerManager().SetTimer(MillisecondsGamingTimer,
	                                       this,
	                                       &UWCPP_Level::UpdateMillisecondsGamingTimerForTimeReductionMode,
	                                       0.01f,
	                                       true);

	GetWorld()->GetTimerManager().SetTimer(SecondsGamingTimer,
	                                       this,
	                                       &UWCPP_Level::UpdateTimerTextBlocksForTimeReductionMode,
	                                       1.0f,
	                                       true,
	                                       1.0f - SecondsInGame);
}

void UWCPP_Level::UpdateMillisecondsGamingTimerForTimeReductionMode()
{
	SecondsInGame += 0.01f;
	SecondsLeft -= 0.01f;
	if (SecondsLeft <= 0.0f && IsValid(CardsDelegatesRef))
	{
		if (GetWorld()->GetTimerManager().TimerExists(MillisecondsGamingTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(MillisecondsGamingTimer);
		}
		UpdateTimerTextBlocksForTimeReductionMode();
		CardsDelegatesRef->GameOverDelegate.ExecuteIfBound();
	}
}

void UWCPP_Level::UpdateTimerTextBlocksForTimeReductionMode()
{
	SecondsTextBlock->SetText(UCPP_StaticLibrary::GetSecondsToDisplayFromSecondsNumber(SecondsLeft));
	const int32 RoundSeconds = UKismetMathLibrary::Round(SecondsLeft);
	MinutesTextBlock->SetText(UCPP_StaticLibrary::GetMinutesToDisplayFromRoundSecondsNumber(RoundSeconds));
}

float UWCPP_Level::ResetGamingTimers()
{
	if (GetWorld()->GetTimerManager().TimerExists(PreviewTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(PreviewTimer);
	}
	if (GetWorld()->GetTimerManager().IsTimerActive(LevelCanBeShownTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(LevelCanBeShownTimer);
	}
	if (GetWorld()->GetTimerManager().TimerExists(MillisecondsGamingTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(MillisecondsGamingTimer);
	}
	if (GetWorld()->GetTimerManager().TimerExists(SecondsGamingTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(SecondsGamingTimer);
	}
	return SecondsInGame;
}

void UWCPP_Level::SetAllPairsNumber(int32& NewValue)
{
	AllPairsNumberTextBlock->SetText(FText::AsNumber(NewValue));
	AllPairsNumber = NewValue;
}

void UWCPP_Level::IncrementFoundPairsNumber()
{
	FoundPairsNumberTextBlock->SetText(FText::AsNumber(++FoundPairsNumber));
	if (CurrentLevelMode == ELevelMode::TimeReduction && AllPairsNumber != FoundPairsNumber)
	{
		SecondsLeft += SecondsToAdd;
		ED_PlayAddSeconds.Broadcast();
		UpdateTimerTextBlocksForTimeReductionMode();
	}
}

void UWCPP_Level::SetFoundPairsNumber(int32& NewValue)
{
	FoundPairsNumberTextBlock->SetText(FText::AsNumber(NewValue));
	FoundPairsNumber = NewValue;
}
