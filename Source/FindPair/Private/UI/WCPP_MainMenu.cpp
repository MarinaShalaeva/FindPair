// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/UI/WCPP_MainMenu.h"

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "FindPair/Classes/UI/CPP_StaticWidgetLibrary.h"
#endif

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UWCPP_MainMenu::UWCPP_MainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                              ConfigureLevelParams_Widget(nullptr),
                                                                              Settings_Widget(nullptr),
                                                                              PlayerResults_Widget(nullptr),
                                                                              GameInfo_Widget(nullptr),
                                                                              UpInscriptionTextBlock(nullptr),
                                                                              PanelsWidgetSwitcher(nullptr),
                                                                              ConfigureLevelParamsSizeBox(nullptr),
                                                                              SettingsSizeBox(nullptr),
                                                                              PlayerResultsSizeBox(nullptr),
                                                                              GameInfoSizeBox(nullptr),
                                                                              LevelParamsPanelButton(nullptr),
                                                                              SettingsButton(nullptr),
                                                                              PlayerResultsButton(nullptr),
                                                                              InfoButton(nullptr),
                                                                              ExitButton(nullptr),
                                                                              GoBackButton(nullptr),
                                                                              DeleteSaveButton(nullptr),
                                                                              DeletingInfo_TB(nullptr),
                                                                              GameInstanceRef(nullptr),
                                                                              PlayerControllerRef(nullptr),
                                                                              WidgetBlueprintsDataTable(nullptr)
{
}

void UWCPP_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstanceRef = GetWorld()->GetGameInstance<UCPP_GameInstance>();
	PlayerControllerRef = GetOwningPlayer();

	if (!MainMenuInscription.IsEmpty())
	{
		ChangeUpInscriptionText(MainMenuInscription);
	}

	ChangeGoBackButtonVisibility(false);

	if (IsValid(LevelParamsPanelButton))
	{
		LevelParamsPanelButton->OnClicked.AddUniqueDynamic(this, &UWCPP_MainMenu::SwitchToConfigureLevelParamsPanel);
	}

	if (IsValid(SettingsButton))
	{
		SettingsButton->OnClicked.AddUniqueDynamic(this, &UWCPP_MainMenu::SwitchToSettingsPanel);
	}

	if (IsValid(PlayerResultsButton))
	{
		PlayerResultsButton->OnClicked.AddUniqueDynamic(this, &UWCPP_MainMenu::SwitchToPlayerResultsPanel);
	}

	if (IsValid(InfoButton))
	{
		InfoButton->OnClicked.AddUniqueDynamic(this, &UWCPP_MainMenu::SwitchToInfoPanel);
	}

	if (IsValid(DeleteSaveButton))
	{
		DeleteSaveButton->OnClicked.AddUniqueDynamic(this, &UWCPP_MainMenu::DeleteSaveButtonOnClicked);
	}

	if (IsValid(ExitButton))
	{
		ExitButton->OnClicked.AddUniqueDynamic(this, &UWCPP_MainMenu::ExitButtonOnClicked);
	}

	if (IsValid(GoBackButton))
	{
		GoBackButton->OnClicked.AddUniqueDynamic(this, &UWCPP_MainMenu::GoBackButtonOnClicked);
	}
}

void UWCPP_MainMenu::NativeDestruct()
{
	if (IsValid(LevelParamsPanelButton) && LevelParamsPanelButton->OnClicked.IsAlreadyBound(
		this, &UWCPP_MainMenu::SwitchToConfigureLevelParamsPanel))
	{
		LevelParamsPanelButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::SwitchToConfigureLevelParamsPanel);
	}

	if (IsValid(SettingsButton) && SettingsButton->OnClicked.IsAlreadyBound(
		this, &UWCPP_MainMenu::SwitchToSettingsPanel))
	{
		SettingsButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::SwitchToSettingsPanel);
	}

	if (IsValid(PlayerResultsButton) && PlayerResultsButton->OnClicked.IsAlreadyBound(
		this, &UWCPP_MainMenu::SwitchToPlayerResultsPanel))
	{
		PlayerResultsButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::SwitchToPlayerResultsPanel);
	}

	if (IsValid(InfoButton) && InfoButton->OnClicked.IsAlreadyBound(this, &UWCPP_MainMenu::SwitchToInfoPanel))
	{
		InfoButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::SwitchToInfoPanel);
	}

	if (IsValid(DeleteSaveButton) && DeleteSaveButton->OnClicked.IsAlreadyBound(
		this, &UWCPP_MainMenu::DeleteSaveButtonOnClicked))
	{
		DeleteSaveButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::DeleteSaveButtonOnClicked);
	}

	if (IsValid(GoBackButton) && GoBackButton->OnClicked.IsAlreadyBound(this, &UWCPP_MainMenu::GoBackButtonOnClicked))
	{
		GoBackButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::GoBackButtonOnClicked);
	}

	if (IsValid(ExitButton) && ExitButton->OnClicked.IsAlreadyBound(this, &UWCPP_MainMenu::ExitButtonOnClicked))
	{
		ExitButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::ExitButtonOnClicked);
	}

	ClearShowDeletingInfoTimer();

	DestroyConfigureLevelParamsWidget();
	DestroySettingsWidget();
	DestroyInfoWidget();

	GameInstanceRef = nullptr;
	PlayerControllerRef = nullptr;

	Super::NativeDestruct();
}

void UWCPP_MainMenu::SwitchToConfigureLevelParamsPanel()
{
	if (PanelsWidgetSwitcher->GetNumWidgets() > 1)
	{
		InitConfigureLevelParamsWidget();

		if (!ChooseLevelInscription.IsEmpty())
		{
			ChangeUpInscriptionText(ChooseLevelInscription);
		}

		PanelsWidgetSwitcher->SetActiveWidgetIndex(1);

		ChangeGoBackButtonVisibility(true);
	}
}

void UWCPP_MainMenu::InitConfigureLevelParamsWidget()
{
	if (ConfigureLevelParams_Widget == nullptr &&
		IsValid(PlayerControllerRef) &&
		IsValid(GameInstanceRef))
	{
		FName Row = FName(TEXT("LevelParams"));
		UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row
			)
		);
		if (Class)
		{
			ConfigureLevelParams_Widget = CreateWidget<UWCPP_ConfigureLevelParams>(PlayerControllerRef, Class);
			if (IsValid(ConfigureLevelParams_Widget))
			{
				ConfigureLevelParams_Widget->SetFlags(RF_StrongRefOnFrame);
				ConfigureLevelParamsSizeBox->AddChild(ConfigureLevelParams_Widget);
			}
		}
	}
}

void UWCPP_MainMenu::DestroyConfigureLevelParamsWidget()
{
	if (IsValid(ConfigureLevelParams_Widget))
	{
		ConfigureLevelParams_Widget->RemoveFromParent();
	}
	ConfigureLevelParams_Widget = nullptr;
}

void UWCPP_MainMenu::SwitchToSettingsPanel()
{
	if (PanelsWidgetSwitcher->GetNumWidgets() > 2)
	{
		InitSettingsWidget();

		if (!SettingsInscription.IsEmpty())
		{
			ChangeUpInscriptionText(SettingsInscription);
		}

		PanelsWidgetSwitcher->SetActiveWidgetIndex(2);

		ChangeGoBackButtonVisibility(true);
	}
}

void UWCPP_MainMenu::InitSettingsWidget()
{
	if (Settings_Widget == nullptr &&
		IsValid(PlayerControllerRef) &&
		IsValid(GameInstanceRef))
	{
		FName Row = FName(TEXT("Settings"));
		UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row
			)
		);
		if (Class)
		{
			Settings_Widget = CreateWidget<UWCPP_Settings>(PlayerControllerRef, Class);
			if (IsValid(Settings_Widget))
			{
				Settings_Widget->SetFlags(RF_StrongRefOnFrame);
				SettingsSizeBox->AddChild(Settings_Widget);
			}
		}
	}
}

void UWCPP_MainMenu::DestroySettingsWidget()
{
	if (IsValid(Settings_Widget))
	{
		Settings_Widget->RemoveFromParent();
	}
	Settings_Widget = nullptr;
}

void UWCPP_MainMenu::SwitchToPlayerResultsPanel()
{
	if (PanelsWidgetSwitcher->GetNumWidgets() > 3)
	{
		InitPlayerResultsWidget();

		if (!PlayerResultsInscription.IsEmpty())
		{
			ChangeUpInscriptionText(PlayerResultsInscription);
		}

		PanelsWidgetSwitcher->SetActiveWidgetIndex(3);

		ChangeGoBackButtonVisibility(true);
	}
}

void UWCPP_MainMenu::InitPlayerResultsWidget()
{
	if (PlayerResults_Widget == nullptr &&
		IsValid(PlayerControllerRef) &&
		IsValid(GameInstanceRef))
	{
		FName Row = FName(TEXT("Results"));
		UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row
			)
		);
		if (Class)
		{
			PlayerResults_Widget = CreateWidget<UWCPP_PlayerResults>(PlayerControllerRef, Class);
			if (IsValid(PlayerResults_Widget))
			{
				PlayerResults_Widget->SetFlags(RF_StrongRefOnFrame);
				PlayerResultsSizeBox->AddChild(PlayerResults_Widget);
			}
		}
	}
}

void UWCPP_MainMenu::DestroyPlayerResultsWidget()
{
	if (IsValid(PlayerResults_Widget))
	{
		PlayerResults_Widget->RemoveFromParent();
	}
	PlayerResults_Widget = nullptr;
}

void UWCPP_MainMenu::SwitchToInfoPanel()
{
	if (PanelsWidgetSwitcher->GetNumWidgets() > 4)
	{
		InitInfoWidget();

		if (!GameInfoInscription.IsEmpty())
		{
			ChangeUpInscriptionText(GameInfoInscription);
		}

		PanelsWidgetSwitcher->SetActiveWidgetIndex(4);

		ChangeGoBackButtonVisibility(true);
	}
}

void UWCPP_MainMenu::InitInfoWidget()
{
	if (GameInfo_Widget == nullptr &&
		IsValid(PlayerControllerRef) &&
		IsValid(GameInstanceRef))
	{
		FName Row = FName(TEXT("GameInfo"));
		UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row
			)
		);
		if (Class)
		{
			GameInfo_Widget = CreateWidget<UWCPP_GameInfo>(PlayerControllerRef, Class);
			if (IsValid(GameInfo_Widget))
			{
				GameInfo_Widget->SetFlags(RF_StrongRefOnFrame);
				GameInfoSizeBox->AddChild(GameInfo_Widget);
			}
		}
	}
}

void UWCPP_MainMenu::DestroyInfoWidget()
{
	if (IsValid(GameInfo_Widget))
	{
		GameInfo_Widget->RemoveFromParent();
	}
	GameInfo_Widget = nullptr;
}

void UWCPP_MainMenu::DeleteSaveButtonOnClicked()
{
	if (!GameInstanceRef->GetSaveManager()->bSaveGameObjectIsDeclared)
	{
		DeletingInfo_TB->SetText(CannotDeleteSaveInscription);
		ClearShowDeletingInfoTimer();
		GetWorld()->GetTimerManager().SetTimer(ShowDeletingInfoTimer,
		                                       this,
		                                       &UWCPP_MainMenu::ClearDeletingInfo,
		                                       2.5f,
		                                       false);
		return;
	}

	if (GameInstanceRef->DeleteSaveGameFileDelegate.IsBound() &&
		GameInstanceRef->DeleteSaveGameFileDelegate.Execute())
	{
		DestroyConfigureLevelParamsWidget();
		DestroyPlayerResultsWidget();

		if (GEngine)
		{
			GEngine->ForceGarbageCollection();
		}

		DeletingInfo_TB->SetText(SaveWasDeletedSuccessfullyInscription);
		ClearShowDeletingInfoTimer();
		GetWorld()->GetTimerManager().SetTimer(ShowDeletingInfoTimer,
		                                       this,
		                                       &UWCPP_MainMenu::ClearDeletingInfo,
		                                       2.5f,
		                                       false);
	}
	else
	{
		DeletingInfo_TB->SetText(CannotDeleteSaveInscription);
		ClearShowDeletingInfoTimer();
		GetWorld()->GetTimerManager().SetTimer(ShowDeletingInfoTimer,
		                                       this,
		                                       &UWCPP_MainMenu::ClearDeletingInfo,
		                                       2.5f,
		                                       false);
	}
}

void UWCPP_MainMenu::ClearDeletingInfo()
{
	DeletingInfo_TB->SetText(FText());
	ClearShowDeletingInfoTimer();
}

void UWCPP_MainMenu::ClearShowDeletingInfoTimer()
{
	if (IsValid(GetWorld()) &&
		GetWorld()->GetTimerManager().TimerExists(ShowDeletingInfoTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(ShowDeletingInfoTimer);
	}
}

void UWCPP_MainMenu::GoBackButtonOnClicked()
{
	PanelsWidgetSwitcher->SetActiveWidgetIndex(0);

	if (!MainMenuInscription.IsEmpty())
	{
		ChangeUpInscriptionText(MainMenuInscription);
	}

	ChangeGoBackButtonVisibility(false);
}

void UWCPP_MainMenu::ExitButtonOnClicked()
{
	if (IsValid(PlayerControllerRef))
	{
		UKismetSystemLibrary::QuitGame(PlayerControllerRef->GetWorld(),
		                               PlayerControllerRef,
		                               EQuitPreference::Quit,
		                               false);
	}
	else
	{
		const UWorld* World = GetWorld();
		UKismetSystemLibrary::QuitGame(World,
		                               UGameplayStatics::GetPlayerController(World, 0),
		                               EQuitPreference::Quit,
		                               false);
	}
}

void UWCPP_MainMenu::ChangeGoBackButtonVisibility(bool bShouldBeVisible)
{
	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(GoBackButton, bShouldBeVisible);

	if (bShouldBeVisible)
	{
		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(GoBackButton, ESlateVisibility::Visible);
	}
	else
	{
		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(GoBackButton, ESlateVisibility::Collapsed);
	}
}

void UWCPP_MainMenu::ChangeUpInscriptionText(const FText& NewText) const
{
	UpInscriptionTextBlock->SetText(NewText);
}
