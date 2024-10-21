// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#ifndef CPP_PARENTCARD_H
#define CPP_PARENTCARD_H
#include "FindPair/Classes/Cards/CPP_ParentCard.h"
#endif
class ACPP_ParentCard;

#ifndef CPP_STATICLIBRARY_H
#define CPP_STATICLIBRARY_H
#include "FindPair/Classes/CPP_StaticLibrary.h"
#endif

#include "CPP_CardsDelegates.generated.h"

/**
 * Delegate for notifying that player decided to start new
 * level.
 */
DECLARE_DELEGATE_OneParam(FShouldStartLevel, ELevelMode /* CurrentLevelMode */)

/**
 * Delegate for notifying the level widget that player decided
 * to restart current level.
 */
DECLARE_DELEGATE(FShouldRestartLevel)

/**
 * Delegate for notifying the Game State that all needed
 * properties should be resetted to successfully restart the
 * level.
 */
DECLARE_DELEGATE(FResetCountersBeforeRestartingLevel)

/**
 * Delegate for notifying that player opened one card on
 * the map.
 */
DECLARE_DELEGATE_OneParam(FCardWasOpenedByPlayer, ACPP_ParentCard* /* OpenedCard */)

/**
 * Delegate for notifying that player tries to close opened
 * card.
 */
DECLARE_DELEGATE_OneParam(FPlayerTriesToCloseCard, ACPP_ParentCard* /* ClosedCard */)

/**
 * Delegate for notifying that the cards have been placed on
 * the map.
 *
 * The Preview Time param means the number of seconds during
 * which all the cards on the map will be opened. After this
 * period of time, the cards on the map will close and the
 * game will start.
 */
DECLARE_DELEGATE_ThreeParams(FCardsSpawningEnded,
                             int32 /* NumberOfPairs */,
                             float /* PreviewTime */,
                             ELevelMode /* CurrentLevelMode */);

/**
 * Delegate for notifying that the player opened two
 * suitable cards (found one another pair).
 */
DECLARE_DELEGATE(FOnePairWasFound);

/**
 * Delegate for notifying that the player decided to skip
 * preview period (period of time intended for remember
 * cards' locations).
 */
DECLARE_DELEGATE(FSkipCardsPreview);

/**
 * Delegate for getting number of seconds that was spent on
 * searching for all the cards.
 */
DECLARE_DELEGATE_RetVal(float /* Seconds spent on searching for all the cards */,
                        FGetTimeInGameAfterFindingAllPairs)

/**
 * Delegate for notifying the Spawn Manager that all cards,
 * located on the map, should be destroyed.
 */
DECLARE_DELEGATE(FShouldDestroyAllCards);

/** Delegate for notifying that player failed this level. */
DECLARE_DELEGATE(FGameOver);

/**
 * Class for storing delegates, that may be needed to work
 * with cards.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class FINDPAIR_API UCPP_CardsDelegates : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * The delegate for notifying that player decided to
	 * start new level.
	 */
	FShouldStartLevel ShouldStartLevelDelegate;

	/**
	 * The delegate for notifying that player decided to
	 * restart the level.
	 */
	FShouldRestartLevel ShouldRestartLevelDelegate;

	/**
	 * Delegate for notifying that all needed properties
	 * should be resetted to successfully restart the
	 * level.
	 */
	FResetCountersBeforeRestartingLevel ResetCountersBeforeRestartingLevelDelegate;

	/**
	 * The delegate for notifying that player opened one
	 * card on the map.
	 */
	FCardWasOpenedByPlayer CardWasOpenedByPlayerDelegate;

	/**
	 * The delegate for notifying that player tries to close
	 * opened card.
	 */
	FPlayerTriesToCloseCard PlayerTriesToCloseCardDelegate;

	/**
	 * The delegate for notifying that the cards have been
	 * placed on the map.
	 */
	FCardsSpawningEnded CardsSpawningEndedDelegate;

	/**
	 * The delegate for notifying that the player opened
	 * two suitable cards (found one another pair).
	 */
	FOnePairWasFound OnePairWasFoundDelegate;

	/**
	 * The delegate for notifying that the player decided
	 * to skip preview period (period of time intended for
	 * remember cards' locations).
	 */
	FSkipCardsPreview SkipCardsPreviewDelegate;

	/**
	 * The delegate for getting number of seconds that was
	 * spent on searching for all the cards.
	 */
	FGetTimeInGameAfterFindingAllPairs GetTimeInGameAfterFindingAllPairsDelegate;

	/**
	 * The delegate for notifying the Spawn Manager that
	 * all cards, located on the map, should be destroyed.
	 */
	FShouldDestroyAllCards ShouldDestroyAllCardsDelegate;

	/**
	 * The delegate for notifying that player failed this
	 * level.
	 */
	FGameOver GameOverDelegate;
};
