// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/UI/WCPP_Pause.h"

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "FindPair/Classes/UI/CPP_StaticWidgetLibrary.h"
#endif

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UWCPP_Pause::UWCPP_Pause(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                        Settings_Widget(nullptr),
                                                                        UpInscriptionTextBlock(nullptr),
                                                                        PanelsWidgetSwitcher(nullptr),
                                                                        SettingsSizeBox(nullptr),
                                                                        ReturnToGameButton(nullptr),
                                                                        RestartGameButton(nullptr),
                                                                        SettingsButton(nullptr),
                                                                        GoToMainMenuButton(nullptr),
                                                                        ExitButton(nullptr),
                                                                        GoBackButton(nullptr),
                                                                        GameInstanceRef(nullptr),
                                                                        PlayerControllerRef(nullptr),
                                                                        WidgetBlueprintsDataTable(nullptr)
{
}

void UWCPP_Pause::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstanceRef = GetWorld()->GetGameInstance<UCPP_GameInstance>();
	PlayerControllerRef = GetOwningPlayer();

	if (IsValid(SettingsButton))
	{
		SettingsButton->OnClicked.AddUniqueDynamic(this, &UWCPP_Pause::SwitchToSettingsPanel);
	}

	if (IsValid(ExitButton))
	{
		ExitButton->OnClicked.AddUniqueDynamic(this, &UWCPP_Pause::ExitButtonOnClicked);
	}

	if (IsValid(GoBackButton))
	{
		ChangeGoBackButtonVisibility(false);
		GoBackButton->OnClicked.AddUniqueDynamic(this, &UWCPP_Pause::GoBackButtonOnClicked);
	}
}

void UWCPP_Pause::NativeDestruct()
{
	if (IsValid(SettingsButton) && SettingsButton->OnClicked.IsAlreadyBound(this, &UWCPP_Pause::SwitchToSettingsPanel))
	{
		SettingsButton->OnClicked.RemoveDynamic(this, &UWCPP_Pause::SwitchToSettingsPanel);
	}

	if (IsValid(GoBackButton) && GoBackButton->OnClicked.IsAlreadyBound(this, &UWCPP_Pause::GoBackButtonOnClicked))
	{
		GoBackButton->OnClicked.RemoveDynamic(this, &UWCPP_Pause::GoBackButtonOnClicked);
	}

	if (IsValid(ExitButton) && ExitButton->OnClicked.IsAlreadyBound(this, &UWCPP_Pause::ExitButtonOnClicked))
	{
		ExitButton->OnClicked.RemoveDynamic(this, &UWCPP_Pause::ExitButtonOnClicked);
	}
	
	DestroySettingsWidget();

	GameInstanceRef = nullptr;
	PlayerControllerRef = nullptr;

	Super::NativeDestruct();
}

void UWCPP_Pause::SwitchToSettingsPanel()
{
	if (PanelsWidgetSwitcher->GetNumWidgets() > 1)
	{
		InitSettingsWidget();

		if (!SettingsInscription.IsEmpty())
		{
			ChangeUpInscriptionText(SettingsInscription);
		}

		PanelsWidgetSwitcher->SetActiveWidgetIndex(1);

		ChangeGoBackButtonVisibility(true);
	}
}

void UWCPP_Pause::InitSettingsWidget()
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

void UWCPP_Pause::DestroySettingsWidget()
{
	if (IsValid(Settings_Widget))
	{
		Settings_Widget->RemoveFromParent();
	}
	Settings_Widget = nullptr;
}

void UWCPP_Pause::GoBackButtonOnClicked()
{
	PanelsWidgetSwitcher->SetActiveWidgetIndex(0);

	if (!PauseInscription.IsEmpty())
	{
		ChangeUpInscriptionText(PauseInscription);
	}

	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(GoBackButton, false);
	UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(GoBackButton, ESlateVisibility::Collapsed);
}

void UWCPP_Pause::ChangeGoBackButtonVisibility(bool bShouldBeVisible)
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

void UWCPP_Pause::ExitButtonOnClicked()
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

void UWCPP_Pause::ChangeUpInscriptionText(const FText& NewText) const
{
	UpInscriptionTextBlock->SetText(NewText);
}
