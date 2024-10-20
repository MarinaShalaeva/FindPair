// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#ifndef CPP_STATICLIBRARY_H
#define CPP_STATICLIBRARY_H
#include "FindPair/Classes/CPP_StaticLibrary.h"
#endif

#ifndef CPP_SAVEGAME_H
#define CPP_SAVEGAME_H
#include "FindPair/Classes/SaveGame/CPP_SaveGame.h"
#endif
class UCPP_SaveGame;

#include "CPP_SaveManager.generated.h"

/**
 * Class for working with the Save Game object
 * (saving and loading game information, creating
 * and deleting save file, etc).
 */
UCLASS()
class FINDPAIR_API UCPP_SaveManager : public UObject
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	UCPP_SaveManager();

	/** Reference to the instance of UCPP_SaveGame class. */
	UPROPERTY()
	UCPP_SaveGame* SaveGameRef;

	/**
	 * The name of the file that stores saved information
	 * about the game.
	 */
	UPROPERTY()
	FString SaveFileName;

public:
	/** Is variable SaveGameRef valid now? */
	UPROPERTY()
	bool bSaveGameObjectIsDeclared;

	/**
	 * Setter for the SaveGameRef variable.
	 * @param NewSaveGame Value to set.
	 */
	UFUNCTION()
	void SetSaveGameObject(UCPP_SaveGame* NewSaveGame);

	/** Getter for the SaveGameRef variable. */
	UFUNCTION()
	UCPP_SaveGame* GetSaveGameObject() const;

	/**
	 * Function for creating a Save Game object with
	 * information loaded from the save file, if the file
	 * exists. Otherwise, a new Save Game object will be
	 * created with variables that store default values.
	 * @return Loaded ot created UCPP_SaveGame instance.
	 */
	UFUNCTION()
	UCPP_SaveGame* LoadOrCreateSaveGameObject();

	/**
	 * Function for saving new player's result to the save
	 * file.
	 * @param bSuccess Player won?
	 * @param NumberOfPairs Number of pairs that were spawned
	 * for this level.
	 * @param LevelMode Current level mode.
	 * @param ClicksNumber Number of cards' spins performed
	 * during playing this level.
	 * @param Seconds Number of seconds that was spent for
	 * completing the level.
	 */
	UFUNCTION()
	void TryToSavePlayerResult(bool bSuccess, int32 NumberOfPairs,
	                           ELevelMode LevelMode,
	                           uint32 ClicksNumber, float Seconds);

	/**
	 * Function for loading player's results from the save
	 * file.
	 * @param Results Array of saved player's results.
	 * @return Loading information from file was successful?
	 */
	bool GetSavedPlayerResults(TArray<struct FPlayerResult>& Results);

	/**
	 * Function for saving last used level's settings to the
	 * save file.
	 * @param NumberOfPairs The last chosen number of pairs.
	 * @param LevelMode The last chosen level mode.
	 * @param CardsStyles The last chosen cards' styles.
	 */
	void TryToSaveNewLevelSettings(int32 NumberOfPairs, ELevelMode LevelMode,
	                               TArray<ECardsStyles> CardsStyles);

	/**
	 * Function for loading last used level's settings from
	 * the save file.
	 * @param NumberOfPairs The last chosen number of pairs.
	 * @param LevelMode The last chosen level mode.
	 * @param CardsStyles The last chosen cards' styles.
	 * @return Loading information from file was successful?
	 */
	bool GetSavedLevelSettings(int32& NumberOfPairs, ELevelMode& LevelMode,
	                           TArray<ECardsStyles>& CardsStyles);

	/**
	 * Function for saving current Save Game object settings
	 * to file.
     * @return Saving process was successful?
	 */
	bool SaveGameInfoToFile();

	/**
	 * Function for deleting current save game file.
	 * @return Deleting was successful?
	 */
	UFUNCTION()
	bool DeleteSave();
};
