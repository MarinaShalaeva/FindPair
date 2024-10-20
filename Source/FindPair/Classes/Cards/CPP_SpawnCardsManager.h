// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#ifndef CPP_GAMEINSTANCE_H
#define CPP_GAMEINSTANCE_H
#include "FindPair/Classes/GameMode/CPP_GameInstance.h"
#endif
class UCPP_GameInstance;

#ifndef CPP_PARENTCARD_H
#define CPP_PARENTCARD_H
#include "FindPair/Classes/Cards/CPP_ParentCard.h"
#endif
class ACPP_ParentCard;

#include "CPP_SpawnCardsManager.generated.h"

/**
 * Class for spawning cards in game world before starting
 * the new level.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class FINDPAIR_API UCPP_SpawnCardsManager : public UObject
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	UCPP_SpawnCardsManager();

public:
	/**
	 * Function for setting new value to CardsDelegatesRef
	 * variable and for binding necessary delegates.
	 * @param NewCardsDelegatesRef CardsDelegates object
	 * reference.
	 */
	UFUNCTION(BlueprintCallable)
	void InitializeCardsDelegatesVariable(UCPP_CardsDelegates* NewCardsDelegatesRef);

protected:
	/** Called before destroying the object. */
	virtual void BeginDestroy() override;

private:
	/**
	 * Reference to the instance of UCPP_CardsDelegates
	 * class.
	 */
	UPROPERTY()
	UCPP_CardsDelegates* CardsDelegatesRef;

	/** Reference to the instance of UCPP_GameInstance class. */
	UPROPERTY()
	UCPP_GameInstance* GameInstanceRef;

	/**
	 * Delegate handle to an object-function pair bound to a
	 * CanShowGeneratedLevelDelegate.
	 */
	FDelegateHandle CanShowGeneratedLevelHandle;

public:
	/**
	 * Array of cards' classes that can be used on current
	 * level.
	 */
	TArray<TSoftClassPtr<ACPP_ParentCard>> AllCardsClasses;

private:
	/**
	 * Array of cards' classes that will be used on current
	 * level to spawn cards on the map.
	 */
	TArray<TSoftClassPtr<ACPP_ParentCard>> CurrentLevelCardsOrder;

public:
	/**
	 * The player wants to play this level using several
	 * cards' styles?
	 */
	bool bSeveralCardsStylesAreUsed = false;

	/**
	 * Cards' backs to use if there are several used cards'
	 * styles.
	 */
	UPROPERTY()
	UMaterialInstance* MI_GeneralCardBack = nullptr;

	/**
	 * The number of cards' pairs that will be used on
	 * level.
	 */
	int32 NumberOfPairs;

	/**
	 * Number of seconds in preview period, after which cards
	 * should become closed.
	 */
	float TimeToClose;

private:
	/**
	 * Array of cards' instances that were spawned for the
	 * current level. 
	 */
	UPROPERTY()
	TArray<ACPP_ParentCard*> GeneratedCards;

	/** Timer for turning the cards upside down. */
	FTimerHandle CloseCardsTimer;

	/** Current level mode. */
	ELevelMode CurrentLevelMode;

	/**
	 * Function for starting new level.
	 * @param LevelMode Current level mode.
	 */
	void StartLevel(ELevelMode LevelMode);

	/** Function for restarting current level. */
	void RestartLevel();

	/** Function for destroying all cards on the map. */
	void DestroyAllCards();

	/**
	 * Function for selecting a random list of usable card
	 * classes and for creating a random cards' order.
	 */
	void RandomizeCards();

	/**
	 * Fisher–Yates shuffle modern algorithm for random
	 * permutation of array elements.
	 * @param Arr An array whose elements need to be
	 * shuffled.
	 */
	static void FisherYatesShuffleAlgorithm(TArray<TSoftClassPtr<ACPP_ParentCard>>& Arr, int32 ArrLen);

	/** Function for spawning cards on the map. */
	void SpawnCards();

	/**
	 * Function for finding all appropriate methods for
	 * spawning certain number of cards.
	 * @param SuitableSpawnStyles Spawning styles that can
	 * be used for such number of cards.
	 * @param LinesNumberRelatedToSpawnStyles An array
	 * containing the number of lines related to the
	 * corresponding spawn methods.
	 * @param MaxCardsNumInLine Maximum number of cards that
	 * can be placed on one line.
	 * @param MaxLinesNum Maximum number of lines.
	 * @param Scale Each card's scale.
	 * @param StepY Horizontal distance between two cards.
	 * @param StepZ Vertical distance between two cards.
	 * @param AllCardsNumber Number of cards that should be
	 * spawned on the map.
	 */
	void FindSuitableSpawningMethods(TArray<int32>& SuitableSpawnStyles,
	                                 TArray<int32>& LinesNumberRelatedToSpawnStyles,
	                                 int32& MaxCardsNumInLine, int32& MaxLinesNum,
	                                 float& Scale, float& StepY, float& StepZ,
	                                 const int32& AllCardsNumber) const;

	/**
	 * Function for choosing spawning method and spawning
	 * cards on the map.
	 * @param SuitableSpawnStyles Spawning styles that can
	 * be used for such number of cards.
	 * @param LinesNumberRelatedToSpawnStyles An array
	 * containing the number of lines related to the
	 * corresponding spawn methods.
	 * @param MaxCardsNumInLine Maximum number of cards that
	 * can be placed on one line.
	 * @param MaxLinesNum Maximum number of lines.
	 * @param Scale Each card's scale.
     * @param StepY Horizontal distance between two cards.
	 * @param StepZ Vertical distance between two cards.
	 * @param AllCardsNumber Number of cards that should be
	 * spawned on the map.
	 */
	void SpawnCardsByRandomMethod(TArray<int32>& SuitableSpawnStyles,
	                              TArray<int32>& LinesNumberRelatedToSpawnStyles,
	                              const int32& MaxCardsNumInLine, const int32& MaxLinesNum,
	                              const float& Scale,
	                              const float& StepY, const float& StepZ,
	                              const int32& AllCardsNumber);

	/**
	 * Function for finding the nearest larger number of
	 * cards that can use Rectangle spawn method.
	 * Is used when all available spawn methods are not
	 * suitable for such a number of cards.
	 * @param Difference Difference between found suitable
	 * number of cards and needed number of cards.
	 * @param SuitableCardsNum Number of cards in line that
	 * suits found number of cards in Equal Lines spawn
	 * method.
	 * @param SuitableLinesNum Number of lines that suits
	 * found number of cards in Equal Lines spawn method.
	 * @param AllCardsNumber Number of cards that should be
	 * spawned on the map.
	 * @param MaxLinesNum Maximum number of lines.
 	 * @param MaxCardsNumInLine Maximum number of cards that
 	 * can be placed on one line.
	 * @return Success.
	 */
	static bool FindNearestLargerCardsNumberWithRectangleSpawnMethod(int32& Difference, int32& SuitableCardsNum,
	                                                                 int32& SuitableLinesNum,
	                                                                 const int32& AllCardsNumber,
	                                                                 const int32& MaxLinesNum,
	                                                                 const int32& MaxCardsNumInLine);

	/**
	 * Function for Subtractive Spawn method. This method is
	 * similar to the Equal Lines Spawn method, but it
	 * removes one or several cards in the first and last
	 * line(s).
	 * Is used when all available spawn methods are not
	 * suitable for such a number of cards.
	 * @param Scale Each card's scale.
	 * @param StepY Horizontal distance between two cards.
	 * @param StepZ Vertical distance between two cards.
	 * @param Difference The number of cards to be removed
	 * in the lines.
	 * @param LinesNum Number of lines.
	 * @param CardsNumForRectangle The number of cards needed
	 * for the Rectangle Spawn method with the same number
	 * of lines and columns.
	 * @param AllCardsNumber Number of cards that should be
	 * spawned on the map.
	 */
	void SubtractiveSpawn(const float& Scale,
	                      const float& StepY, const float& StepZ,
	                      const int32& Difference, const int32& LinesNum,
	                      const int32& CardsNumForRectangle,
	                      const int32& AllCardsNumber);

	/**
	 * Function for spawning cards in the shape of a
	 * rectangle.
	 * @param Scale Each card's scale.
	 * @param StepY Horizontal distance between two cards.
	 * @param StepZ Vertical distance between two cards.
	 * @param LinesNumber Number of lines.
	 * @param AllCardsNumber Number of cards that should be
	 * spawned on the map.
	 */
	void RectangleSpawn(const float& Scale,
	                    const float& StepY, const float& StepZ,
	                    const int32& LinesNumber,
	                    const int32& AllCardsNumber);

	/**
	 * Function for spawning cards in the shape of
	 * herringbone (in even and odd lines, the number of
	 * cards will differ by one).
	 * @param Scale Each card's scale.
	 * @param StepY Horizontal distance between two cards.
	 * @param StepZ Vertical distance between two cards.
	 * @param LinesNumber Number of lines.
	 * @param AllCardsNumber Number of cards that should be
	 * spawned on the map.
	 */
	void HerringboneSpawn(const float& Scale,
	                      const float& StepY, const float& StepZ,
	                      const int32& LinesNumber,
	                      const int32& AllCardsNumber);

	/**
	 * Function to search for the largest cards' number that
	 * can be spawned in the «biggest» line in Decreasing
	 * Hourglass spawn method (in the first and the last
	 * lines).
	 * @param LinesNumber Number of lines.
	 * @param AllCardsNumber Number of cards that should be
	 * spawned on the map.
	 * @return Maximum number of cards (number of cards in
	 * the «biggest» line).
	 */
	static int32 FindMaxCardsNumInLineForDecreasingHourglassSpawn(const int32& LinesNumber,
	                                                              const int32& AllCardsNumber);

	/**
	 * Function for spawning cards in the shape of classic
	 * hourglass (in the first and in the last lines there
	 * will be the largest number of cards, in the second
	 * and in the penultimate lines number of cards will be
	 * less by one, etc; in the central line number of cards
	 * will be the smallest).
	 * @param Scale Each card's scale.
	 * @param StepY Horizontal distance between two cards.
	 * @param StepZ Vertical distance between two cards.
	 * @param LinesNumber Number of lines.
	 * @param AllCardsNumber Number of cards that should be
	 * spawned on the map.
	 */
	void DecreasingHourglassSpawn(const float& Scale,
	                              const float& StepY, const float& StepZ,
	                              const int32& LinesNumber,
	                              const int32& AllCardsNumber);

	/**
	 * Function to search for the largest cards' number that
	 * can be spawned in the «biggest» line in Increasing
	 * Hourglass spawn method (in the central line).
	 * @param LinesNumber Number of lines.
	 * @param AllCardsNumber Number of cards that should be
	 * spawned on the map.
	 * @return Maximum number of cards (number of cards in
	 * the «biggest» line).
	 */
	static int32 FindMaxCardsNumInLineForIncreasingHourglassSpawn(const int32& LinesNumber,
	                                                              const int32& AllCardsNumber);

	/**
	 * Function for spawning cards in the shape of «inverted»
	 * hourglass (in the first and in the last lines there
	 * will be the smallest number of cards, in the second
	 * and in the penultimate lines number of cards will be
	 * greater by one, etc; in the central line number of
	 * cards will be the largest).
	 * @param Scale Each card's scale.
	 * @param StepY Horizontal distance between two cards.
	 * @param StepZ Vertical distance between two cards.
	 * @param LinesNumber Number of lines.
	 * @param AllCardsNumber Number of cards that should be
	 * spawned on the map.
	 */
	void IncreasingHourglassSpawn(const float& Scale,
	                              const float& StepY, const float& StepZ,
	                              const int32& LinesNumber,
	                              const int32& AllCardsNumber);

	/**
	 * Function for spawning one card on the map.
	 * @param Scale Card's scale.
	 * @param CoordinateY Y coordinate to spawn.
	 * @param CoordinateZ Z coordinate to spawn.
	 * @param Index Index of needed element in Current Level
	 * Cards Order array.
	 */
	void SpawnCardOnLevel(const float& Scale,
	                      const float& CoordinateY,
	                      const float& CoordinateZ,
	                      const int32& Index);

	/**
	 * Function with response on start showing generated
	 * level. Calls timer for preview period.
	 */
	void StartCloseAllCardsTimer();

	/** Function for turning all cards upside down. */
	void CloseAllCards();
};
