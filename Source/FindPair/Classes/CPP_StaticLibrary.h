// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CPP_StaticLibrary.generated.h"

/** Enumeration for the level modes. */
UENUM()
enum class ELevelMode : uint8
{
	/**
	 * In this mode time, which was spent on the current
	 * level, will only grow up. The game does not hurry the
	 * user and allows him to enjoy the process, at the end
	 * showing the result. Player always wins.
	 */
	Classic,
	/**
	 * In this mode, the user has a limited amount of time
	 * to find all the pairs on the map. The time will
	 * increase by a few seconds after successfully finding
	 * one pair.
	 */
	TimeReduction
};

/** Enumeration with all available cards' styles. */
UENUM()
enum class ECardsStyles : uint8
{
	None,
	Halloween,
	Autumn,
	Winter,
	Bakery,
	Sport,
	Cleaning,
	Covid,
	Flowers,
	Spring,
	Summer,
	Valentines,
	School
};

/**
 * Structure for storing information about one successfully
 * ended game level.
 */
USTRUCT(BlueprintType)
struct FPlayerResult
{
	GENERATED_BODY()

	/** Number of pairs, that were on the level. */
	UPROPERTY(SaveGame)
	int32 NumberOfPairs;

	/** Used level mode. */
	UPROPERTY(SaveGame)
	ELevelMode LevelMode;

	/** Number of cards' spins during playing this level. */
	UPROPERTY(SaveGame)
	uint32 NumberOfClicks;

	/**
	 * Number of seconds that was spent for completing the
	 * level.
	 */
	UPROPERTY(SaveGame)
	float Seconds;

	/**
	 * Seconds that were spent for completing the level,
	 * represented in format HH:MM:SS.
	 */
	UPROPERTY(SaveGame)
	FText TimeResultAsText;

	/**
	 * An empty constructor that initializes the fields of
	 * the structure with default values.
	 */
	FPlayerResult()
	{
		NumberOfPairs = 10;
		LevelMode = ELevelMode::Classic;
		NumberOfClicks = 0;
		Seconds = 0.0f;
		TimeResultAsText = FText();
	}

	/**
	 * A constructor with parameters to initialize all fields
	 * of the structure from the outside.
	 */
	FPlayerResult(const int32 InNumberOfPairs, const ELevelMode InLevelMode,
	              const uint32 InNumberOfClicks, const float InSeconds,
	              const FText& InTimeResultAsText)
	{
		NumberOfPairs = InNumberOfPairs;
		LevelMode = InLevelMode;
		NumberOfClicks = InNumberOfClicks;
		Seconds = InSeconds;
		TimeResultAsText = InTimeResultAsText;
	}
};

/** Library with static functions. */
UCLASS()
class FINDPAIR_API UCPP_StaticLibrary : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * A function for getting a float value as FText up to a
	 * certain decimal place.
	 * @param TheFloat Input float value.
	 * @param Precision Number of decimal places.
	 * @param IncludeLeadingZero Should there be a zero before
	 * the comma in the result if the modulus of the number
	 * is less than one?
	 * @return Resulted FText value.
	 */
	static FText GetFloatAsTextWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero);

	/**
	 * Function for getting number of seconds in format HH:MM:SS.
	 * @param SpentSeconds Number of seconds.
	 * @return Number of seconds in format HH:MM:SS.
	 */
	static FText GetTextRepresentationOfSpentSeconds(float SpentSeconds);

	/**
	 * Function for obtaining separately the whole part and
	 * the remainder of a float number.
	 * @param TheFloat Input float value.
	 * @param Precision Number of decimal places.
	 * @param OutWholePart A whole part of the value.
	 * @param OutReminder A reminder of the value.
	 */
	static void GetWholePartAndRemainderAsText(float TheFloat, int32 Precision,
	                                           FText& OutWholePart, FText& OutReminder);

	/**
	 * Function for getting only displaying seconds number
	 * from number of seconds (for time in format HH:MM:SS).
	 * @param Seconds Number of seconds.
	 * @return Seconds number as FText.
	 */
	static FText GetSecondsToDisplayFromSecondsNumber(float Seconds);

	/**
	 * Function for getting only displaying minutes number
	 * from number of seconds (for time in format HH:MM:SS).
	 * @param Seconds Number of seconds.
	 * @return Minutes number as FText.
	 */
	static FText GetMinutesToDisplayFromSecondsNumber(float Seconds);

	/**
	 * Function for getting only displaying minutes number
	 * from rounded number of seconds (for time in format
	 * HH:MM:SS).
	 * @param RoundSeconds Number of seconds.
	 * @return Minutes number as FText.
	 */
	static FText GetMinutesToDisplayFromRoundSecondsNumber(int32 RoundSeconds);
};
