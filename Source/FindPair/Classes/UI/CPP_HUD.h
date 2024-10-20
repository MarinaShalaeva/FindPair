// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/DataTable.h"

#ifndef CPP_PLAYERCONTROLLER_H
#define CPP_PLAYERCONTROLLER_H
#include "FindPair/Classes/GameMode/CPP_PlayerController.h"
#endif
class ACPP_PlayerController;

#ifndef WCPP_CONTAINER_H
#define WCPP_CONTAINER_H
#include "FindPair/Classes/UI/WCPP_Container.h"
#endif
class UWCPP_Container;

#ifndef WCPP_GAMEENDEDPANEL_H
#define WCPP_GAMEENDEDPANEL_H
#include "FindPair/Classes/UI/WCPP_GameEndedPanel.h"
#endif
class UWCPP_GameEndedPanel;

#ifndef WCPP_LEVEL_H
#define WCPP_LEVEL_H
#include "FindPair/Classes/UI/WCPP_Level.h"
#endif
class UWCPP_Level;

#ifndef WCPP_PAUSE_H
#define WCPP_PAUSE_H
#include "FindPair/Classes/UI/WCPP_Pause.h"
#endif
class UWCPP_Pause;

#ifndef WCPP_MAINMENU_H
#define WCPP_MAINMENU_H
#include "FindPair/Classes/UI/WCPP_MainMenu.h"
#endif
class UWCPP_MainMenu;

#ifndef WCPP_LOADINGSCREEN_H
#define WCPP_LOADINGSCREEN_H
#include "FindPair/Classes/UI/WCPP_LoadingScreen.h"
#endif
class UWCPP_LoadingScreen;

#include "CPP_HUD.generated.h"

/** Heads-up Display for working with widgets. */
UCLASS()
class FINDPAIR_API ACPP_HUD : public AHUD
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_HUD();

	/**
	 * Function for storing logic that should be applied
	 * when the actor appears in the game world.
	 */
	virtual void BeginPlay() override;

	/**
	 * Function for storing logic that should be applied in
	 * the end of the actor's life (but before its destroying).
	 * @param EndPlayReason Specifies why an actor is being
	 * deleted/removed from a level.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/**
	 * Delegate handle to an object-function pair bound to a
	 * CanShowGeneratedLevelDelegate.
	 */
	FDelegateHandle CanShowGeneratedLevelHandle;

	/**
	 * Delegate handle to an object-function pair bound to a
	 * StartGameDelegate.
	 */
	FDelegateHandle StartGameDelegateHandle;

	/**
	 * Delegate handle to an object-function pair bound to
	 * a LevelWasEndedDelegate.
	 */
	FDelegateHandle LevelWasEndedDelegateHandle;

	/**
	 * Reference to the instance of ACPP_PlayerController
	 * class.
	 */
	UPROPERTY()
	ACPP_PlayerController* PlayerControllerRef;

	/** Reference to the instance of UCPP_GameInstance class. */
	UPROPERTY()
	UCPP_GameInstance* GameInstanceRef;

protected:
	/**
	 * Data Table with the list of soft references to all
	 * widgets' classes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
	UDataTable* WidgetBlueprintsDataTable;

public:
	/** Reference to current Container widget blueprint. */
	UPROPERTY()
	UWCPP_Container* Container_Widget;

	/** Reference to current Main Menu widget blueprint. */
	UPROPERTY()
	UWCPP_MainMenu* MainMenu_Widget;

	/** Reference to current Pause widget blueprint. */
	UPROPERTY()
	UWCPP_Pause* Pause_Widget;

	/** Reference to current Level widget blueprint. */
	UPROPERTY()
	UWCPP_Level* Level_Widget;

	/** Reference to current Game Ended widget blueprint. */
	UPROPERTY()
	UWCPP_GameEndedPanel* GameEnded_Widget;

	/** Reference to current Loading Screen widget blueprint. */
	UPROPERTY()
	UWCPP_LoadingScreen* LoadingScreen_Widget;

	/**
	 * Function for initializing necessary widgets after the
	 * level's starting.
	 * @param PairsNumber Number of pairs on the map.
	 * @param LevelModeToUse Current level mode.
	 * @param CardsStylesToUse Array of cards' styles to use
	 * for spawning cards on the map.
	 */
	UFUNCTION()
	void StartLevel(int32& PairsNumber,
	                ELevelMode& LevelModeToUse,
	                TArray<ECardsStyles>& CardsStylesToUse);

	/**
	 * Function for initializing necessary widgets after the
	 * level's ending.
	 * @param bSuccess Player won?
	 * @param NumberOfPairs Number of pairs on the map.
	 * @param LevelMode Current level mode.
	 * @param ClicksNumber Number of cards'
	 * spins performed during playing this level.
	 * @param SpentSeconds Number of seconds that was spent
	 * for completing the level.
	 */
	UFUNCTION()
	void LevelWasEnded(bool bSuccess, int32 NumberOfPairs,
	                   ELevelMode LevelMode, uint32 ClicksNumber,
	                   float SpentSeconds);

	/** Initializing of the Container widget. */
	void InitializeContainerWidget();

	/** Destroying the Container widget. */
	void DestroyContainerWidget();

	/** Initializing of the Main Menu widget. */
	UFUNCTION()
	void InitializeMainMenuWidget();

	/** Destroying the Main Menu widget. */
	void DestroyMainMenuWidget();

	/** Initializing of the Pause widget. */
	void InitializePauseWidget();

	/** Destroying the Pause widget. */
	UFUNCTION()
	void DestroyPauseWidget();

	/** Initializing of the Level widget. */
	UFUNCTION()
	void InitializeLevelWidget();

	/** Destroying the Level widget. */
	UFUNCTION()
	void DestroyLevelWidget();

	/**
	 * Initializing of the Ended Game widget.
	 * @param ClicksNumber Number of cards' spins performed
	 * during playing this level.
	 * @param SpentSeconds Number of seconds that was spent
	 * for completing the level.
	 * @param bSuccess Player won?
	 */
	void InitializeEndedGameWidget(uint32 ClicksNumber,
	                               float SpentSeconds,
	                               bool bSuccess);

	/** Destroying the Ended Game widget. */
	UFUNCTION()
	void DestroyEndedGameWidget();

	/** Initializing of the Loading Screen widget. */
	UFUNCTION()
	void InitializeLoadingScreenWidget();

	/** Destroying the Loading Screen widget. */
	UFUNCTION()
	void DestroyLoadingScreenWidget();

	/**
	 * Function for creating Pause Widget and for calling
	 * the Player Controller's «Start Pause» function.
	 */
	UFUNCTION()
	void StartPause();

	/**
	 * Function for destroying Pause Widget and for calling
	 * the Player Controller's «End Pause» function.
	 */
	UFUNCTION()
	void EndPause();

	/**
	 * Function for destroying unnecessary widgets and for
	 * calling the Player Controller's «Restart Current Game»
	 * function.
	 */
	UFUNCTION()
	void RestartCurrentGame();

	/**
	 * Function for creating Main Menu widget and for calling
	 * the Player Controller's «Return To Main Menu» function.
	 */
	UFUNCTION()
	void ReturnToMainMenu();
};
