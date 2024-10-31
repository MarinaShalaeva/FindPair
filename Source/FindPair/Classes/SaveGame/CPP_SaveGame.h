// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#ifndef CPP_STATICLIBRARY_H
#define CPP_STATICLIBRARY_H
#include "FindPair/Classes/CPP_StaticLibrary.h"
#endif

#include "CPP_SaveGame.generated.h"

/**
 * Save Game object for saving and loading «Find a pair»
 * game's information.
 */
UCLASS()
class FINDPAIR_API UCPP_SaveGame : public USaveGame
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	UCPP_SaveGame();

public:
	/**
	 * Array with player's successfully ended levels'
	 * results.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Player Results")
	TArray<struct FPlayerResult> Results;

	/** Last chosen by player number of pairs. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Level Settings")
	int32 LastNumberOfPairs;

	/** Last chosen by player level mode. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Level Settings")
	ELevelMode LastLevelMode;

	/** Array of last chosen by player cards' styles. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Level Settings")
	TArray<ECardsStyles> LastChosenCardsStyles;
};
