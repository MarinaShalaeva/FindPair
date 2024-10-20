// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#ifndef CPP_STATICLIBRARY_H
#define CPP_STATICLIBRARY_H
#include "FindPair/Classes/CPP_StaticLibrary.h"
#endif

#ifndef CPP_GAMEINSTANCE_H
#define CPP_GAMEINSTANCE_H
#include "FindPair/Classes/GameMode/CPP_GameInstance.h"
#endif
class UCPP_GameInstance;

#include "Components/Button.h"
class UButton;
#include "Components/TextBlock.h"
class UTextBlock;
#include "Components/CheckBox.h"
class UCheckBox;
#include "Components/Slider.h"
class USlider;

#include "WCPP_ConfigureLevelParams.generated.h"

/**
 * Widget class that contains UI elements for choosing level
 * settings before starting the new game (new level).
 */
UCLASS(Abstract)
class FINDPAIR_API UWCPP_ConfigureLevelParams : public UUserWidget
{
	GENERATED_BODY()

protected:
	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_ConfigureLevelParams(const FObjectInitializer& ObjectInitializer);

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
	/** Reference to the instance of UCPP_GameInstance class. */
	UPROPERTY()
	UCPP_GameInstance* GameInstanceRef;

	/** Can the player start playing a new level? */
	bool bCanStartGame;

protected:
	//=======Cards Checkboxes==================

	/**
	 * Array of checkboxes intended to different cards'
	 * styles.
	 */
	UPROPERTY()
	TArray<UCheckBox*> CardsStylesCheckBoxes;

	/** Checkbox for Halloween style of cards. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* HalloweenCardsCheckBox;

	/** Checkbox for Autumn style of cards. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* AutumnCardsCheckBox;

	/** Checkbox for Winter style of cards. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* WinterCardsCheckBox;

	/** Checkbox for Bakery style of cards. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* BakeryCardsCheckBox;

	/** Checkbox for Sport style of cards. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SportCardsCheckBox;

	/** Checkbox for Cleaning style of cards. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* CleaningCardsCheckBox;

	/** Checkbox for Covid style of cards. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* CovidCardsCheckBox;

	/** Checkbox for Flowers style of cards. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* FlowersCardsCheckBox;

	/** Checkbox for Spring style of cards. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SpringCardsCheckBox;

	/** Checkbox for Summer style of cards. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SummerCardsCheckBox;

	/** Checkbox for Valentines style of cards. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* ValentinesCardsCheckBox;

	/** Checkbox for School style of cards. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SchoolCardsCheckBox;

	//=======Level Mode=======================

	/** Chosen level mode. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	ELevelMode CurrentLevelMode;

	/** Checkbox for classic level mode. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* ClassicModeCheckBox;

	/** Checkbox for time reduction level mode. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* TimeReductionModeCheckBox;

	//=======Pairs Number=====================

	/** Chosen number of pairs. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PairsNumber;

	/** Text block for displaying the number of pairs. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PairsNumberTextBlock;

	/** Slider to change pairs number. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* PairsNumberSlider;

	//=======Buttons=========================

	/**
	 * Button to choose all cards' styles or cancel the
	 * choice of all cards' styles (styles that will be
	 * used during level).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChooseAllCardsStylesButton;

	/** Button for staring the new level. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* StartGameButton;

	//=======Choosing styles texts===========

	/**
	 * Text block that will be display in the center of
	 * ChooseAllCardsStylesButton.
	 * Should indicate what action will be performed when
	 * the button is clicked: choosing all cards' styles or
	 * canceling this selection.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ChooseAllCardsStylesTextBlock;

	/** Inscription for choosing all cards' styles. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ChooseAllCardsStylesText;

	/**
	 * Inscription for canceling the selection of all cards'
	 * styles.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DeleteAllCardsStylesText;

private:
	/**
	 * Does current inscription match the Choose All Cards
	 * Styles Text variable?
	 */
	bool bSelectAllCardsInscriptionIsActive;

	//=======Functions=======================

protected:
	/**
	 * Function to set suitable checkboxes' states to true
	 * by input cards' styles array.
	 * @param CardsStyles Cards' styles that should be
	 * selected.
	 */
	void SelectCardsStylesByArray(TArray<ECardsStyles>& CardsStyles);

	/**
	 * Function for changing Change Pairs Number slider's
	 * value.
	 * @param Value New slider value.
	 */
	UFUNCTION(BlueprintCallable)
	void ChangePairsNum(float Value);

	/** Function for choosing all cards' styles. */
	UFUNCTION()
	void SelectAllCardsStyles();

	/**
	 * Function for canceling the selection of all cards'
	 * styles.
	 */
	UFUNCTION()
	void DeselectAllCardsStyles();

	/**
	 * Function that contains logic that should be applied
	 * when any style checkbox changes its state.
	 * @param bIsChecked Is the new value of checkbox true?
	 */
	UFUNCTION(BlueprintCallable)
	void OneCardsStyleCheckStateChanged(bool bIsChecked);

	/**
	 * Function for changing opportunity to start new level
	 * (changing enabling of Start New Level button).
	 * @param bCanStart Can the player start new level?
	 */
	UFUNCTION()
	void ChangeOpportunityToStartGame(bool bCanStart);

	/**
	 * Function that contains logic that should be applied
	 * when ClassicModeCheckBox changes its state.
	 * @param bIsChecked Is the new value of checkbox true?
	 */
	UFUNCTION(BlueprintCallable)
	void ClassicModeCheckBoxOnCheckStateChanged(bool bIsChecked);

	/**
	 * Function that contains logic that should be applied
	 * when TimeReductionModeCheckBox changes its state.
	 * @param bIsChecked Is the new value of checkbox true?
	 */
	UFUNCTION(BlueprintCallable)
	void TimeReductionModeCheckBoxOnCheckStateChanged(bool bIsChecked);

private:
	/**
	 * Function that contains logic that should be applied
	 * when the player presses the Start Game Button.
	 */
	UFUNCTION()
	void StartGameButtonOnClicked();

public:
	/**
	 * Function that contains logic that should be applied
	 * when the player presses the Choose All Cards Styles
	 * Button.
	 */
	UFUNCTION(BlueprintCallable)
	void ChooseAllCardsStylesButtonOnClicked();

private:
	/**
	 * Function that returns chosen on this panel level's
	 * settings.
	 * @param OutPairsNumber Chosen number of pairs.
	 * @param OutLevelMode Chosen level mode.
	 * @param OutCardsStyles Chosen cards' styles.
	 */
	UFUNCTION()
	void GetCurrentLevelParams(int32& OutPairsNumber,
	                           ELevelMode& OutLevelMode,
	                           TArray<ECardsStyles>& OutCardsStyles);
};
