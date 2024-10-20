// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/HorizontalBox.h"
class UHorizontalBox;
#include "Components/TextBlock.h"
class UTextBlock;
#include "Components/Button.h"
class UButton;
#include "Components/VerticalBox.h"
class UVerticalBox;

#ifndef CPP_CARDSDELEGATES_H
#define CPP_CARDSDELEGATES_H
#include "FindPair/Classes/Cards/CPP_CardsDelegates.h"
#endif
class UCPP_CardsDelegates;

#include "WCPP_Level.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayAddSecondsAnimation);

/**
 * Parent widget class for visualizing information about
 * current level.
 */
UCLASS(Abstract)
class FINDPAIR_API UWCPP_Level : public UUserWidget
{
	GENERATED_BODY()

protected:
	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_Level(const FObjectInitializer& ObjectInitializer);

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

	//============UI Elements=====================

	/**
	 * Horizontal box that contains text blocks with number
	 * of pairs that were found by the player and with all
	 * pairs' number.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UHorizontalBox* PairsNumberHorizontalBox;

	/**
	 * Text block that shows number of pairs that were found
	 * by the player.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* FoundPairsNumberTextBlock;

	/**
	 * Text block that shows general number of pairs that
	 * should be found by the player on this level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* AllPairsNumberTextBlock;

	/**
	 * Text block that shows how many seconds should be added
	 * to the left number of seconds in the time reduction
	 * level mode.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* AddSecondsTextBlock;

	/**
	 * Vertical box that contains UI elements connected with
	 * preview period.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* StartEarlierVerticalBox;

	/**
	 * Text block that shows how many seconds player has to
	 * remember cards' locations.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SecondsBeforeStartTextBlock;

	/** Button for skipping the preview period. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* StartGameEarlierButton;

	/**
	 * Text block that shows how many hours the player has
	 * been playing the current level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* HoursTextBlock;

	/**
	 * Text block that shows a colon symbol between hours
	 * and minutes text blocks.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ColonBetweenHoursAndMinutes;

	/**
	 * Text block that shows how many minutes the player has
	 * been playing the current level in classic mode and how
	 * many minutes are left in time reduction mode.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MinutesTextBlock;

	/**
	 * Text block that shows how many seconds the player has
	 * been playing the current level in classic mode and how
	 * many seconds are left in time reduction mode.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SecondsTextBlock;

public:
	/** Button to pause the game. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PauseButton;

private:
	//============Objects references==============
	/**
	 * Reference to the instance of UCPP_CardsDelegates
	 * class.
	 */
	UPROPERTY()
	UCPP_CardsDelegates* CardsDelegatesRef;

	//======Preview period variables==============

	/** Timer for preview period. */
	FTimerHandle PreviewTimer;
	/** How many seconds of preview period left. */
	float PreviewTime;

	//============Timers==========================

	/** Timer to calculate milliseconds in game. */
	FTimerHandle MillisecondsGamingTimer;
	/** Timer to calculate seconds in game. */
	FTimerHandle SecondsGamingTimer;
	/** How many seconds the player is playing this level. */
	float SecondsInGame;
	/**
	 * How many seconds left to play this level in time
	 * reduction level mode.
	 */
	float SecondsLeft;
	/**
	 * How many seconds should be added to the left number
	 * of seconds in the time reduction level mode.
	 */
	float SecondsToAdd;
	/**
	 * Is it necessary to display the number of hours spent
	 * at this level at the moment? (Is the number of hours
	 * at this level bigger than 0?) 
	 */
	bool bShouldPrintHours;

	//============Number of pairs=================

	/**
	 * Variable that shows number of pairs that were found
	 * by the player.
	 */
	int32 FoundPairsNumber;

	/**
	 * Variable that shows general number of pairs that
	 * should be found by the player on this level.
	 */
	int32 AllPairsNumber;

	/** Current level mode. */
	ELevelMode CurrentLevelMode;

	//============Delegates=======================

protected:
	/**
	 * Delegate intended to show how many seconds were added
	 * to number of left seconds in time reduction level mode.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
	FPlayAddSecondsAnimation ED_PlayAddSeconds;

public:
	/**
	 * Function for setting new value to CardsDelegatesRef
	 * variable and for binding necessary delegates.
	 * @param NewCardsDelegatesRef CardsDelegates object
	 * reference.
	 */
	UFUNCTION()
	void InitializeCardsDelegatesVariable(UCPP_CardsDelegates*& NewCardsDelegatesRef);

private:
	/**
	 * Function for starting playing this level.
	 * @param AllPairsNum General number of pairs that should
	 * be found by the player on this level.
	 * @param LocalPreviewTime The number of seconds that the
	 * player has at his disposal in order to remember the
	 * location of the cards on the map.
	 * @param LevelMode Chosen by the player level mode.
	 */
	void StartNewGame(int32 AllPairsNum, float LocalPreviewTime, ELevelMode LevelMode);

	/**
	 * Timer for notification that the level can be shown to
	 * the user.
	 */
	FTimerHandle LevelCanBeShownTimer;

	/**
	 * Function that notifies other classes that the level
	 * was generated and could be shown to the player.
	 */
	void LevelCanBeShownToUserNotification();

	/**
	 * Function for starting playing this level in classic
	 * mode.
	 */
	void StartNewGameForClassicMode();
	/**
	 * Function for starting playing this level in time
	 * reduction mode.
	 */
	void StartNewGameForTimeReductionMode();

	/** Function intended to update preview period timer. */
	void UpdatePreviewTimer();
	/**
	 * Function intended to finish the preview period, during
	 * which player can try to remember cards' locations on
	 * the map.
	 */
	void FinishPreviewPeriod();
	/**
	 * Function intended to skip the preview period, during
	 * which player can try to remember cards' locations on
	 * the map. Function is called when the player clicks on
	 * StartGameEarlierButton.
	 */
	UFUNCTION()
	void SkipPreviewPeriod();

	/** Function intended to start all necessary times. */
	void ActivateGamingTimer();
	/**
	 * Function intended to start times related to classic
	 * level mode.
	 */
	void ActivateGamingTimerForClassicMode();
	/**
	 * Function intended to start times related to time
	 * reduction level mode.
	 */
	void ActivateGamingTimerForTimeReductionMode();
	/**
	 * Function which is called every millisecond.
	 * Intended to update necessary variables in time
	 * reduction level mode.
	 */
	void UpdateMillisecondsGamingTimerForTimeReductionMode();
	/**
	 * Function which is called every second.
	 * Intended to update necessary variables in time
	 * reduction level mode.
	 */
	void UpdateTimerTextBlocksForTimeReductionMode();

	/**
	 * Function which is called every millisecond.
	 * Intended to update necessary variables in classic
	 * level mode.
	 */
	void UpdateMillisecondsGamingTimerForClassicMode();
	/**
	 * Function which is called every second.
	 * Intended to update necessary variables in classic
	 * level mode.
	 */
	void UpdateTimerTextBlocksForClassicMode();
	/**
	 * Function intended to update hours text block.
	 * @param RoundSeconds Rounded number of seconds.
	 */
	void UpdateHoursTextBlock(int32 RoundSeconds);
	/**
	 * Function for clearing all existing in this class
	 * timers.
	 * @return Number of seconds spent on playing this level.
	 */
	float ResetGamingTimers();

	/**
	 * Function for updating AllPairsNumber variable and the
	 * associated text block.
	 * @param NewValue New AllPairsNumber value.
	 */
	void SetAllPairsNumber(int32& NewValue);
	/**
	 * Function for increasing FoundPairsNumber by one and
	 * updating the associated text block.
	 */
	void IncrementFoundPairsNumber();
	/**
	 * Function for updating FoundPairsNumber variable and
	 * the associated text block.
	 * @param NewValue New FoundPairsNumber value.
	 */
	void SetFoundPairsNumber(int32& NewValue);
};
