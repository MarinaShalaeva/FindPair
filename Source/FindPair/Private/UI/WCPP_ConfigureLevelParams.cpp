// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/UI/WCPP_ConfigureLevelParams.h"

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "FindPair/Classes/UI/CPP_StaticWidgetLibrary.h"
#endif

#ifndef CPP_PLAYERCONTROLLER_H
#define CPP_PLAYERCONTROLLER_H
#include "FindPair/Classes/GameMode/CPP_PlayerController.h"
#endif
class ACPP_PlayerController;

UWCPP_ConfigureLevelParams::UWCPP_ConfigureLevelParams(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	  GameInstanceRef(nullptr),
	  bCanStartGame(false),
	  HalloweenCardsCheckBox(nullptr),
	  AutumnCardsCheckBox(nullptr),
	  WinterCardsCheckBox(nullptr),
	  BakeryCardsCheckBox(nullptr),
	  SportCardsCheckBox(nullptr),
	  CleaningCardsCheckBox(nullptr),
	  CovidCardsCheckBox(nullptr),
	  FlowersCardsCheckBox(nullptr),
	  SpringCardsCheckBox(nullptr),
	  SummerCardsCheckBox(nullptr),
	  ValentinesCardsCheckBox(nullptr),
	  SchoolCardsCheckBox(nullptr),
	  CurrentLevelMode(ELevelMode::Classic),
	  ClassicModeCheckBox(nullptr),
	  TimeReductionModeCheckBox(nullptr),
	  PairsNumber(1),
	  PairsNumberTextBlock(nullptr),
	  PairsNumberSlider(nullptr),
	  ChooseAllCardsStylesButton(nullptr),
	  StartGameButton(nullptr),
	  ChooseAllCardsStylesTextBlock(nullptr),
	  bSelectAllCardsInscriptionIsActive(true)
{
}

void UWCPP_ConfigureLevelParams::NativeConstruct()
{
	Super::NativeConstruct();

	CardsStylesCheckBoxes.Empty(12);

	CardsStylesCheckBoxes.Emplace(HalloweenCardsCheckBox);
	CardsStylesCheckBoxes.Emplace(AutumnCardsCheckBox);
	CardsStylesCheckBoxes.Emplace(WinterCardsCheckBox);
	CardsStylesCheckBoxes.Emplace(BakeryCardsCheckBox);
	CardsStylesCheckBoxes.Emplace(SportCardsCheckBox);
	CardsStylesCheckBoxes.Emplace(CleaningCardsCheckBox);
	CardsStylesCheckBoxes.Emplace(CovidCardsCheckBox);
	CardsStylesCheckBoxes.Emplace(FlowersCardsCheckBox);
	CardsStylesCheckBoxes.Emplace(SpringCardsCheckBox);
	CardsStylesCheckBoxes.Emplace(SummerCardsCheckBox);
	CardsStylesCheckBoxes.Emplace(ValentinesCardsCheckBox);
	CardsStylesCheckBoxes.Emplace(SchoolCardsCheckBox);

	GameInstanceRef = GetWorld()->GetGameInstance<UCPP_GameInstance>();

	if (IsValid(GameInstanceRef) && GameInstanceRef->GetSavedLevelSettingsDelegate.IsBound())
	{
		int32 OutPairsNumber;
		ELevelMode OutLevelMode;
		TArray<ECardsStyles> OutCardsStyles;
		if (GameInstanceRef->GetSavedLevelSettingsDelegate.Execute(OutPairsNumber, OutLevelMode, OutCardsStyles))
		{
			if (OutPairsNumber <= 120 && OutPairsNumber > 0)
			{
				PairsNumberSlider->SetValue(OutPairsNumber);
				ChangePairsNum(OutPairsNumber);
			}

			if (OutLevelMode == ELevelMode::Classic)
			{
				ClassicModeCheckBox->SetCheckedState(ECheckBoxState::Checked);
				ClassicModeCheckBoxOnCheckStateChanged(true);
			}
			else if (OutLevelMode == ELevelMode::TimeReduction)
			{
				TimeReductionModeCheckBox->SetCheckedState(ECheckBoxState::Checked);
				TimeReductionModeCheckBoxOnCheckStateChanged(true);
			}

			if (OutCardsStyles.Num() <= 0)
			{
				DeselectAllCardsStyles();
			}
			else if (OutCardsStyles.Num() == CardsStylesCheckBoxes.Num())
			{
				SelectAllCardsStyles();
			}
			else
			{
				SelectCardsStylesByArray(OutCardsStyles);
			}
		}
		else
		{
			ChangeOpportunityToStartGame(false);
		}
	}
	else
	{
		ChangeOpportunityToStartGame(false);
	}

	StartGameButton->OnClicked.AddDynamic(this, &UWCPP_ConfigureLevelParams::StartGameButtonOnClicked);
}

void UWCPP_ConfigureLevelParams::NativeDestruct()
{
	if (IsValid(StartGameButton) && StartGameButton->OnClicked.IsAlreadyBound(
		this, &UWCPP_ConfigureLevelParams::StartGameButtonOnClicked))
	{
		StartGameButton->OnClicked.RemoveDynamic(this, &UWCPP_ConfigureLevelParams::StartGameButtonOnClicked);
	}
	if (IsValid(GameInstanceRef) && GameInstanceRef->SaveNewLevelSettingsDelegate.IsBound())
	{
		int32 PairsNumberToUse;
		ELevelMode LevelModeToUse;
		TArray<ECardsStyles> CardsStylesToUse;
		GetCurrentLevelParams(PairsNumberToUse, LevelModeToUse, CardsStylesToUse);
		GameInstanceRef->SaveNewLevelSettingsDelegate.Execute(PairsNumberToUse, LevelModeToUse, CardsStylesToUse);
	}

	GameInstanceRef = nullptr;

	Super::NativeDestruct();
}

void UWCPP_ConfigureLevelParams::SelectCardsStylesByArray(TArray<ECardsStyles>& CardsStyles)
{
	for (auto CardsStyle : CardsStyles)
	{
		if (CardsStyle == ECardsStyles::Halloween)
		{
			HalloweenCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else if (CardsStyle == ECardsStyles::Autumn)
		{
			AutumnCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else if (CardsStyle == ECardsStyles::Winter)
		{
			WinterCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else if (CardsStyle == ECardsStyles::Bakery)
		{
			BakeryCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else if (CardsStyle == ECardsStyles::Sport)
		{
			SportCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else if (CardsStyle == ECardsStyles::Cleaning)
		{
			CleaningCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else if (CardsStyle == ECardsStyles::Covid)
		{
			CovidCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else if (CardsStyle == ECardsStyles::Flowers)
		{
			FlowersCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else if (CardsStyle == ECardsStyles::Spring)
		{
			SpringCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else if (CardsStyle == ECardsStyles::Summer)
		{
			SummerCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else if (CardsStyle == ECardsStyles::Valentines)
		{
			ValentinesCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else if (CardsStyle == ECardsStyles::School)
		{
			SchoolCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		OneCardsStyleCheckStateChanged(true);
	}
}

void UWCPP_ConfigureLevelParams::ChangePairsNum(float Value)
{
	PairsNumber = static_cast<int32>(Value);
	PairsNumberTextBlock->SetText(FText::AsNumber(PairsNumber));
}

void UWCPP_ConfigureLevelParams::SelectAllCardsStyles()
{
	HalloweenCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
	AutumnCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
	WinterCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
	BakeryCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
	SportCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
	CleaningCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
	CovidCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
	FlowersCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
	SpringCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
	SummerCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
	ValentinesCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);
	SchoolCardsCheckBox->SetCheckedState(ECheckBoxState::Checked);

	ChooseAllCardsStylesTextBlock->SetText(DeleteAllCardsStylesText);
	bSelectAllCardsInscriptionIsActive = false;
	ChangeOpportunityToStartGame(true);
}

void UWCPP_ConfigureLevelParams::DeselectAllCardsStyles()
{
	HalloweenCardsCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	AutumnCardsCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	WinterCardsCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	BakeryCardsCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	SportCardsCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	CleaningCardsCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	CovidCardsCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	FlowersCardsCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	SpringCardsCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	SummerCardsCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	ValentinesCardsCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	SchoolCardsCheckBox->SetCheckedState(ECheckBoxState::Unchecked);

	ChooseAllCardsStylesTextBlock->SetText(ChooseAllCardsStylesText);
	bSelectAllCardsInscriptionIsActive = true;
	ChangeOpportunityToStartGame(false);
}

void UWCPP_ConfigureLevelParams::OneCardsStyleCheckStateChanged(bool bIsChecked)
{
	if (!bIsChecked)
	{
		bool bCheckedBoxExist = false;
		for (const auto& Box : CardsStylesCheckBoxes)
		{
			if (Box->GetCheckedState() == ECheckBoxState::Checked)
			{
				bCheckedBoxExist = true;
				break;
			}
		}
		if (!bCheckedBoxExist)
		{
			ChangeOpportunityToStartGame(false);
		}
	}
	else
	{
		ChangeOpportunityToStartGame(true);
	}
}

void UWCPP_ConfigureLevelParams::ChangeOpportunityToStartGame(bool bCanStart)
{
	bCanStartGame = bCanStart;
	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(StartGameButton, bCanStartGame);
}

void UWCPP_ConfigureLevelParams::ClassicModeCheckBoxOnCheckStateChanged(bool bIsChecked)
{
	if (bIsChecked)
	{
		CurrentLevelMode = ELevelMode::Classic;
		TimeReductionModeCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		ClassicModeCheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::TimeReductionModeCheckBoxOnCheckStateChanged(bool bIsChecked)
{
	if (bIsChecked)
	{
		CurrentLevelMode = ELevelMode::TimeReduction;
		ClassicModeCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		TimeReductionModeCheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::StartGameButtonOnClicked()
{
	int32 PairsNumberToUse;
	ELevelMode LevelModeToUse;
	TArray<ECardsStyles> CardsStylesToUse;
	GetCurrentLevelParams(PairsNumberToUse, LevelModeToUse, CardsStylesToUse);

	if (IsValid(GameInstanceRef))
	{
		if (GameInstanceRef->SaveNewLevelSettingsDelegate.IsBound())
		{
			GameInstanceRef->SaveNewLevelSettingsDelegate.Execute(PairsNumberToUse, LevelModeToUse, CardsStylesToUse);
		}
		GameInstanceRef->StartGameDelegate.Broadcast(PairsNumberToUse, LevelModeToUse, CardsStylesToUse);
	}
}

void UWCPP_ConfigureLevelParams::ChooseAllCardsStylesButtonOnClicked()
{
	if (bSelectAllCardsInscriptionIsActive)
	{
		SelectAllCardsStyles();
	}
	else
	{
		DeselectAllCardsStyles();
	}
}

void UWCPP_ConfigureLevelParams::GetCurrentLevelParams(int32& OutPairsNumber,
                                                       ELevelMode& OutLevelMode,
                                                       TArray<ECardsStyles>& OutCardsStyles)
{
	OutPairsNumber = PairsNumber;
	OutLevelMode = CurrentLevelMode;

	OutCardsStyles.Empty();

	if (HalloweenCardsCheckBox->GetCheckedState() == ECheckBoxState::Checked)
	{
		OutCardsStyles.Add(ECardsStyles::Halloween);
	}
	if (AutumnCardsCheckBox->GetCheckedState() == ECheckBoxState::Checked)
	{
		OutCardsStyles.Add(ECardsStyles::Autumn);
	}
	if (WinterCardsCheckBox->GetCheckedState() == ECheckBoxState::Checked)
	{
		OutCardsStyles.Add(ECardsStyles::Winter);
	}
	if (BakeryCardsCheckBox->GetCheckedState() == ECheckBoxState::Checked)
	{
		OutCardsStyles.Add(ECardsStyles::Bakery);
	}
	if (SportCardsCheckBox->GetCheckedState() == ECheckBoxState::Checked)
	{
		OutCardsStyles.Add(ECardsStyles::Sport);
	}
	if (CleaningCardsCheckBox->GetCheckedState() == ECheckBoxState::Checked)
	{
		OutCardsStyles.Add(ECardsStyles::Cleaning);
	}
	if (CovidCardsCheckBox->GetCheckedState() == ECheckBoxState::Checked)
	{
		OutCardsStyles.Add(ECardsStyles::Covid);
	}
	if (FlowersCardsCheckBox->GetCheckedState() == ECheckBoxState::Checked)
	{
		OutCardsStyles.Add(ECardsStyles::Flowers);
	}
	if (SpringCardsCheckBox->GetCheckedState() == ECheckBoxState::Checked)
	{
		OutCardsStyles.Add(ECardsStyles::Spring);
	}
	if (SummerCardsCheckBox->GetCheckedState() == ECheckBoxState::Checked)
	{
		OutCardsStyles.Add(ECardsStyles::Summer);
	}
	if (ValentinesCardsCheckBox->GetCheckedState() == ECheckBoxState::Checked)
	{
		OutCardsStyles.Add(ECardsStyles::Valentines);
	}
	if (SchoolCardsCheckBox->GetCheckedState() == ECheckBoxState::Checked)
	{
		OutCardsStyles.Add(ECardsStyles::School);
	}
}
