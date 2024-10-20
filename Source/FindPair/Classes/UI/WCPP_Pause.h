// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"

#include "Components/TextBlock.h"
class UTextBlock;
#include "Components/Button.h"
class UButton;
#include "Components/SizeBox.h"
class USizeBox;
#include "Components/WidgetSwitcher.h"
class UWidgetSwitcher;

#ifndef WCPP_SETTINGS_H
#define WCPP_SETTINGS_H
#include "FindPair/Classes/UI/WCPP_Settings.h"
#endif
class UWCPP_Settings;

#include "WCPP_Pause.generated.h"

/** Parent widget class for the pause menu. */
UCLASS(Abstract)
class FINDPAIR_API UWCPP_Pause : public UUserWidget
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_Pause(const FObjectInitializer& ObjectInitializer);

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

	//=======Widgets classes and instances========

	/** Reference to current Settings widget blueprint. */
	UPROPERTY()
	UWCPP_Settings* Settings_Widget;

	//============UI Elements=====================

protected:
	//============Inscriptions====================
	/**
    * Text block for the caption at the top of the widget,
    * containing information about current opened panel.
    */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* UpInscriptionTextBlock;

	/** Variable containing the pause menu inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText PauseInscription;

	/** Variable containing the settings inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText SettingsInscription;


	//==========Widget Switchers==================

	/**
	 * Widget switcher containing all necessary for the pause
	 * menu panels.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* PanelsWidgetSwitcher;


	//============Size boxes======================

	/** Size box for settings panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* SettingsSizeBox;


	//===============Buttons======================

public:
	/** Function for continuing playing this level. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ReturnToGameButton;

	/** Button for restarting the current level. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RestartGameButton;

protected:
	/** Button for switching to settings panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SettingsButton;

public:
	/** Button for returning to the main menu panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GoToMainMenuButton;

protected:
	/** Button to exit the game. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitButton;

	/**
	 * Button for returning to the main panel, containing
	 * the list of available buttons.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GoBackButton;

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
	 * @param bShouldBeVisible Should the go back button
	 * be visible and enabled?
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
