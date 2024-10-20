// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"

#include "Components/Button.h"
class UButton;
#include "Components/SizeBox.h"
class USizeBox;
#include "Components/TextBlock.h"
class UTextBlock;
#include "Components/WidgetSwitcher.h"
class UWidgetSwitcher;

#ifndef CPP_GAMEINSTANCE_H
#define CPP_GAMEINSTANCE_H
#include "FindPair/Classes/GameMode/CPP_GameInstance.h"
#endif
class UCPP_GameInstance;

#ifndef WCPP_SETTINGS_H
#define WCPP_SETTINGS_H
#include "FindPair/Classes/UI/WCPP_Settings.h"
#endif
class UWCPP_Settings;

#ifndef WCPP_CONFIGURELEVELPARAMS_H
#define WCPP_CONFIGURELEVELPARAMS_H
#include "FindPair/Classes/UI/WCPP_ConfigureLevelParams.h"
#endif
class UWCPP_ConfigureLevelParams;

#ifndef WCPP_PLAYERRESULTS_H
#define WCPP_PLAYERRESULTS_H
#include "FindPair/Classes/UI/WCPP_PlayerResults.h"
#endif
class UWCPP_PlayerResults;

#ifndef WCPP_GAMEINFO_H
#define WCPP_GAMEINFO_H
#include "FindPair/Classes/UI/WCPP_GameInfo.h"
#endif
class UWCPP_GameInfo;

#include "WCPP_MainMenu.generated.h"

/**
 * Parent widget class for the main menu panel.
 */
UCLASS(Abstract)
class FINDPAIR_API UWCPP_MainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_MainMenu(const FObjectInitializer& ObjectInitializer);

	/**
	 * Function that stores logic that should be applied
	 * when the widget is created.
	 */
	virtual void NativeConstruct() override;

	/**
	 * Function that stores logic that should be applied
	 * when the widget starts destroying.
	 */
	virtual void NativeDestruct() override;

private:
	//==========Widgets instances=================

	/**
	 * Reference to current Configure Level Params widget
	 * blueprint.
	 */
	UPROPERTY()
	UWCPP_ConfigureLevelParams* ConfigureLevelParams_Widget;

	/** Reference to current Settings widget blueprint. */
	UPROPERTY()
	UWCPP_Settings* Settings_Widget;

	/** Reference to current Player Results widget blueprint. */
	UPROPERTY()
	UWCPP_PlayerResults* PlayerResults_Widget;

	/** Reference to current Game Info widget blueprint. */
	UPROPERTY()
	UWCPP_GameInfo* GameInfo_Widget;

	//============UI Elements=====================

protected:
	//============Inscriptions====================
	/**
	 * Text block for the caption at the top of the widget,
	 * containing information about current opened panel.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* UpInscriptionTextBlock;

	/** Variable containing the main menu inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText MainMenuInscription;

	/** Variable containing the level's settings inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText ChooseLevelInscription;

	/** Variable containing the settings inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText SettingsInscription;

	/** Variable containing the player's results inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText PlayerResultsInscription;

	/**
	 * Variable containing the information about the game
	 * inscription.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText GameInfoInscription;

	//==========Widget Switchers==================

	/**
	 * Widget switcher containing all necessary for the main
	 * menu panels.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* PanelsWidgetSwitcher;


	//============Size boxes======================

	/** Size box for choosing level's settings panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* ConfigureLevelParamsSizeBox;

	/** Size box for settings panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* SettingsSizeBox;

	/** Size box for player's results panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* PlayerResultsSizeBox;

	/** Size box for information about the game panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* GameInfoSizeBox;


	//===============Buttons======================

	/**
	 * Button for switching to choosing level's settings
	 * panel.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* LevelParamsPanelButton;

	/** Button for switching to settings panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SettingsButton;

	/** Button for switching to the player's results panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayerResultsButton;

	/**
	 * Button for switching to the panel with information
	 * about the game.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* InfoButton;

	/** Button to exit the game. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitButton;

	/**
	 * Button for returning to the main panel, containing
	 * the list of available buttons.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GoBackButton;

	/** Button for deleting the save file. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* DeleteSaveButton;


	//==========Delete save file==================

	/** Inscription for successful deleting of save file. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inscriptions")
	FText SaveWasDeletedSuccessfullyInscription;

	/** Inscription for unsuccessful deleting of save file. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inscriptions")
	FText CannotDeleteSaveInscription;

	/**
	 * Text block for the message about process of deleting
	 * the save file.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* DeletingInfo_TB;


	//================Timers======================

	/**
	 * Timer for showing information about deleting the save
	 * file. 
	 */
	FTimerHandle ShowDeletingInfoTimer;

private:
	//============Objects references==============
	/**
	 * Reference to the instance of UCPP_CardsDelegates
	 * class.
	 */
	UPROPERTY()
	UCPP_GameInstance* GameInstanceRef;

	/** Reference to the instance of APlayerController class. */
	UPROPERTY()
	APlayerController* PlayerControllerRef;

protected:
	/**
	 * Data Table with the list of soft references to all
	 * widgets' classes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
	UDataTable* WidgetBlueprintsDataTable;

	//============Functions=======================

private:
	/**
	 * Function for changing current opened panel to the
	 * Configure Level Params panel.
	 */
	UFUNCTION()
	void SwitchToConfigureLevelParamsPanel();

	/** Function for creating the level's settings panel. */
	UFUNCTION()
	void InitConfigureLevelParamsWidget();

	/** Function for destroying the level's settings panel. */
	UFUNCTION()
	void DestroyConfigureLevelParamsWidget();

	/**
	 * Function for changing current opened panel to the
	 * settings panel.
	 */
	UFUNCTION()
	void SwitchToSettingsPanel();

	/** Function for creating the settings panel. */
	UFUNCTION()
	void InitSettingsWidget();

	/** Function for destroying the settings panel. */
	UFUNCTION()
	void DestroySettingsWidget();

	/**
	 * Function for changing current opened panel to the
	 * player's results panel.
	 */
	UFUNCTION()
	void SwitchToPlayerResultsPanel();

	/** Function for creating the player's results panel. */
	UFUNCTION()
	void InitPlayerResultsWidget();

	/** Function for destroying the player's results panel. */
	UFUNCTION()
	void DestroyPlayerResultsWidget();

	/**
	 * Function for changing current opened panel to the
	 * information about the game panel.
	 */
	UFUNCTION()
	void SwitchToInfoPanel();

	/** Function for creating the game's information panel. */
	UFUNCTION()
	void InitInfoWidget();

	/** Function for destroying the game's information panel. */
	UFUNCTION()
	void DestroyInfoWidget();

	/**
	 * Function for deleting the save file. Function is
	 * called after clicking on Delete Save Button.
	 */
	UFUNCTION()
	void DeleteSaveButtonOnClicked();

	/**
	 * Function for hide the message about process of
	 * deleting the save file.
	 */
	UFUNCTION()
	void ClearDeletingInfo();

	/** Function for clearing the Show Deleting Info Timer. */
	UFUNCTION()
	void ClearShowDeletingInfoTimer();

	/**
	 * Function for returning to the main panel, containing
	 * the list of available buttons.
	 * Function is called after clicking on Go Back Button.
	 */
	UFUNCTION()
	void GoBackButtonOnClicked();

	/**
	 * Function intended to exit the game.
	 * Function is called after clicking on Exit Button.
	 */
	UFUNCTION()
	void ExitButtonOnClicked();

	/**
	 * Function changing go back button's visibility and
	 * enabling.
	 * @param bShouldBeVisible Should the go back button be
	 * visible and enabled?
	 */
	UFUNCTION()
	void ChangeGoBackButtonVisibility(bool bShouldBeVisible);

	/**
	 * Function for changing caption at the top of the
	 * widget.
	 * @param NewText Inscription to set.
	 */
	UFUNCTION()
	void ChangeUpInscriptionText(const FText& NewText) const;
};
