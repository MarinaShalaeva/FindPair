// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/Cards/CPP_SpawnCardsManager.h"
#include "Kismet/GameplayStatics.h"

UCPP_SpawnCardsManager::UCPP_SpawnCardsManager()
{
	GameInstanceRef = nullptr;
	CardsDelegatesRef = nullptr;
	CurrentLevelMode = ELevelMode::Classic;
	NumberOfPairs = 0;
	TimeToClose = 0.0f;
}

void UCPP_SpawnCardsManager::InitializeCardsDelegatesVariable(UCPP_CardsDelegates* NewCardsDelegatesRef)
{
	if (IsValid(NewCardsDelegatesRef))
	{
		CardsDelegatesRef = NewCardsDelegatesRef;
		CardsDelegatesRef->ShouldStartLevelDelegate.BindUObject(this, &UCPP_SpawnCardsManager::StartLevel);
		CardsDelegatesRef->ShouldRestartLevelDelegate.BindUObject(this, &UCPP_SpawnCardsManager::RestartLevel);
		CardsDelegatesRef->SkipCardsPreviewDelegate.BindUObject(this, &UCPP_SpawnCardsManager::CloseAllCards);
		CardsDelegatesRef->ShouldDestroyAllCardsDelegate.BindUObject(this, &UCPP_SpawnCardsManager::DestroyAllCards);
	}
	GameInstanceRef = Cast<UCPP_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (IsValid(GameInstanceRef))
	{
		CanShowGeneratedLevelHandle = GameInstanceRef->CanShowGeneratedLevelDelegate.AddUObject(
			this, &UCPP_SpawnCardsManager::StartCloseAllCardsTimer);
	}
}

void UCPP_SpawnCardsManager::BeginDestroy()
{
	if (IsValid(GameInstanceRef))
	{
		if (CanShowGeneratedLevelHandle.IsValid())
		{
			GameInstanceRef->CanShowGeneratedLevelDelegate.Remove(CanShowGeneratedLevelHandle);
			CanShowGeneratedLevelHandle.Reset();

			GameInstanceRef = nullptr;
		}
	}
	UObject::BeginDestroy();
}

void UCPP_SpawnCardsManager::StartLevel(ELevelMode LevelMode)
{
	CurrentLevelMode = LevelMode;

	if (NumberOfPairs <= 0 || NumberOfPairs > 120)
		return;

	RandomizeCards();

	GeneratedCards.Empty();

	SpawnCards();

	if (NumberOfPairs >= 1 && NumberOfPairs <= 3)
	{
		TimeToClose = 1.0f;
	}
	else if (NumberOfPairs >= 4 && NumberOfPairs <= 6)
	{
		TimeToClose = 1.5f;
	}
	else if (NumberOfPairs >= 7 && NumberOfPairs <= 9)
	{
		TimeToClose = 2.0f;
	}
	else if (NumberOfPairs >= 10 && NumberOfPairs <= 12)
	{
		TimeToClose = 2.5f;
	}
	else if (NumberOfPairs >= 13 && NumberOfPairs <= 15)
	{
		TimeToClose = 3.0f;
	}
	else if (NumberOfPairs >= 16 && NumberOfPairs <= 18)
	{
		TimeToClose = 3.5f;
	}
	else if (NumberOfPairs >= 19 && NumberOfPairs <= 21)
	{
		TimeToClose = 4.0f;
	}
	else if (NumberOfPairs >= 22 && NumberOfPairs <= 23)
	{
		TimeToClose = 4.5f;
	}
	else
	{
		TimeToClose = 5.0f;
	}

	if (CardsDelegatesRef->CardsSpawningEndedDelegate.IsBound())
	{
		CardsDelegatesRef->CardsSpawningEndedDelegate.Execute(NumberOfPairs, TimeToClose, CurrentLevelMode);
	}
}

void UCPP_SpawnCardsManager::RestartLevel()
{
	StartLevel(CurrentLevelMode);
}

void UCPP_SpawnCardsManager::DestroyAllCards()
{
	if (GeneratedCards.Num() <= 0)
	{
		TArray<AActor*> Arr;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_ParentCard::StaticClass(), Arr);

		for (const auto& Actor : Arr)
		{
			Actor->Destroy();
		}
	}
	else
	{
		for (const auto& Actor : GeneratedCards)
		{
			Actor->Destroy();
		}
	}

	GeneratedCards.Empty();
}

void UCPP_SpawnCardsManager::RandomizeCards()
{
	CurrentLevelCardsOrder.Empty();

	const int32 Len = AllCardsClasses.Num();
	const int32 ForLoopIterationNumber = NumberOfPairs / Len;
	const int32 Remainder = NumberOfPairs % Len;

	for (int32 i = 0; i < ForLoopIterationNumber * 2; i++)
	{
		CurrentLevelCardsOrder.Append(AllCardsClasses);
	}

	// If there aren't any Remainder (if we
	// need exactly all existing classes or
	// if we need a number of classes that
	// is a multiple of the total number of
	// existing ones), than we don't need to
	// randomize anything.
	if (Remainder == 0)
	{
		FisherYatesShuffleAlgorithm(CurrentLevelCardsOrder, CurrentLevelCardsOrder.Num());
		return;
	}

	// Fisher–Yates shuffle modern algorithm.
	TArray<TSoftClassPtr<ACPP_ParentCard>> AllRandomClasses = AllCardsClasses;
	FisherYatesShuffleAlgorithm(AllRandomClasses, AllRandomClasses.Num());

	// Now we'll add as many first elements of
	// the array of Random Classes as we got
	// in the Remainder.
	for (int32 i = 0; i < Remainder; i++)
	{
		CurrentLevelCardsOrder.Emplace(AllRandomClasses[i]);
		CurrentLevelCardsOrder.Emplace(AllRandomClasses[i]);
	}

	FisherYatesShuffleAlgorithm(CurrentLevelCardsOrder, CurrentLevelCardsOrder.Num());
}

void UCPP_SpawnCardsManager::FisherYatesShuffleAlgorithm(TArray<TSoftClassPtr<ACPP_ParentCard>>& Arr, int32 ArrLen)
{
	const int32 Len = Arr.Num();
	for (int32 i = 0; i <= Len - 2; i++)
	{
		const int32 j = FMath::RandRange(i, Len - 1);
		const TSoftClassPtr<ACPP_ParentCard> Tmp = Arr[i];
		Arr[i] = Arr[j];
		Arr[j] = Tmp;
		// Arr.Swap(i, j);
	}
}

void UCPP_SpawnCardsManager::SpawnCards()
{
	if (GetWorld()->GetTimerManager().TimerExists(CloseCardsTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(CloseCardsTimer);
	}

	// 0 - «Equal lines» (or «Rectangle»)
	// Spawning method.
	// 1 - «Herringbone» Spawning:
	// o o o o o o
	//  o o o o o
	// o o o o o o
	//  o o o o o
	// ====or====
	//  o o o o o
	// o o o o o o
	//  o o o o o
	// o o o o o o
	// 2 - Decreasing «Hourglass» Spawning:
	// o o o o o o
	//  o o o o o
	//   o o o o
	//  o o o o o
	// o o o o o o
	// 3 - Increasing «Hourglass» Spawning:
	//   o o o o
	//  o o o o o
	// o o o o o o
	//  o o o o o
	//   o o o o
	TArray<int32> SuitableSpawnStyles;
	TArray<int32> LinesNumberRelatedToSpawnStyles;

	const int32 AllCardsNumber = CurrentLevelCardsOrder.Num();

	float Scale, StepY, StepZ;
	int32 MaxCardsNumInLine, MaxLinesNum;

	FindSuitableSpawningMethods(SuitableSpawnStyles, LinesNumberRelatedToSpawnStyles,
	                            MaxCardsNumInLine, MaxLinesNum,
	                            Scale, StepY, StepZ, AllCardsNumber);

	SpawnCardsByRandomMethod(SuitableSpawnStyles, LinesNumberRelatedToSpawnStyles,
	                         MaxCardsNumInLine, MaxLinesNum,
	                         Scale, StepY, StepZ, AllCardsNumber);
}

void UCPP_SpawnCardsManager::FindSuitableSpawningMethods(TArray<int32>& SuitableSpawnStyles,
                                                         TArray<int32>& LinesNumberRelatedToSpawnStyles,
                                                         int32& MaxCardsNumInLine, int32& MaxLinesNum,
                                                         float& Scale, float& StepY, float& StepZ,
                                                         const int32& AllCardsNumber) const
{
	if (NumberOfPairs <= 10)
	{
		MaxCardsNumInLine = 7;
		MaxLinesNum = 3;
		Scale = 1.0f;
		StepY = 50.0f;
		StepZ = 50.0f;
	}
	else if (NumberOfPairs <= 18)
	{
		MaxCardsNumInLine = 9;
		MaxLinesNum = 4;
		Scale = 0.75f;
		StepY = 38.75f;
		StepZ = 37.0f;
	}
	else if (NumberOfPairs <= 26)
	{
		MaxCardsNumInLine = 11;
		MaxLinesNum = 5;
		Scale = 0.625f;
		StepY = 30.0f;
		StepZ = 29.125f;
	}
	else if (NumberOfPairs <= 36 ||
		NumberOfPairs <= 45 && AllCardsNumber % 6 == 0)
	{
		MaxCardsNumInLine = 15;
		MaxLinesNum = 6;
		Scale = 0.5f;
		StepY = 23.0f;
		StepZ = 23.0f;
	}
	else if (NumberOfPairs <= 68)
	{
		MaxCardsNumInLine = 17;
		MaxLinesNum = 8;
		Scale = 0.375f;
		StepY = 18.0f;
		StepZ = 18.0f;
	}
	else
	{
		MaxCardsNumInLine = 24;
		MaxLinesNum = 10;
		Scale = 0.3f;
		StepY = 14.0f;
		StepZ = 14.0f;
	}

	// If the maximum possible number of lines is
	// greater than or equal to 5, then we can
	// consider the possibility of using «hourglass»
	// spawning method. If the maximum number of
	// lines is less than 5, then this option will
	// coincide with the «herringbone» spawning
	// method and it is pointless to consider it.
	if (MaxLinesNum >= 5)
	{
		const int32 LinesNumberForSuchSpawningMethod = MaxLinesNum % 2 == 0 ? MaxLinesNum - 1 : MaxLinesNum;

		// Set 1 to variable because of card in circle line.
		int32 MinCardsNumForDecreasingHourglassSpawning = 1;
		int32 DecreasingCardCounter = 2;
		for (int32 i = LinesNumberForSuchSpawningMethod / 2; i >= 1; i--)
		{
			MinCardsNumForDecreasingHourglassSpawning += DecreasingCardCounter * 2;
			DecreasingCardCounter++;
		}

		if ((AllCardsNumber - MinCardsNumForDecreasingHourglassSpawning) % LinesNumberForSuchSpawningMethod == 0
			&& FindMaxCardsNumInLineForDecreasingHourglassSpawn(LinesNumberForSuchSpawningMethod, AllCardsNumber)
			<= MaxCardsNumInLine)
		{
			SuitableSpawnStyles.Add(2);
			LinesNumberRelatedToSpawnStyles.Add(LinesNumberForSuchSpawningMethod);
		}

		int32 MinCardsNumForIncreasingHourglassSpawning = 0;
		int32 IncreasingCardCounter = 1;
		for (int32 i = 1; i <= LinesNumberForSuchSpawningMethod / 2; i++)
		{
			MinCardsNumForIncreasingHourglassSpawning += IncreasingCardCounter * 2;
			IncreasingCardCounter += 1;
		}
		MinCardsNumForIncreasingHourglassSpawning += IncreasingCardCounter;

		if ((AllCardsNumber - MinCardsNumForIncreasingHourglassSpawning) % LinesNumberForSuchSpawningMethod == 0 &&
			FindMaxCardsNumInLineForIncreasingHourglassSpawn(LinesNumberForSuchSpawningMethod, AllCardsNumber)
			<= MaxCardsNumInLine)
		{
			SuitableSpawnStyles.Add(3);
			LinesNumberRelatedToSpawnStyles.Add(LinesNumberForSuchSpawningMethod);
		}
	}

	// Check for the possibility of using
	// «Equal Lines» (or «Rectangle») spawning
	// method.
	if (AllCardsNumber % MaxLinesNum == 0 && AllCardsNumber / MaxLinesNum <= MaxCardsNumInLine)
	{
		SuitableSpawnStyles.Add(0);
		LinesNumberRelatedToSpawnStyles.Add(MaxLinesNum);
	}

	if (AllCardsNumber % (MaxLinesNum - 1) == 0 &&
		AllCardsNumber / (MaxLinesNum - 1) <= MaxCardsNumInLine)
	{
		SuitableSpawnStyles.Add(0);
		LinesNumberRelatedToSpawnStyles.Add(MaxLinesNum - 1);
	}

	// Check for the possibility of using
	// «Herringbone» spawning method.
	if (AllCardsNumber >= MaxLinesNum)
	{
		if (MaxLinesNum % 2 == 1)
		{
			// If the desired number of lines is odd.

			// In this case, we will be satisfied with
			// the remainder of dividing the desired
			// number of lines by 2, equal to any integer
			// close to the real float number obtained
			// after division (that is, if the result of
			// division is 2.5, the remainder equal to 2
			// and 3 will suit us).
			if (AllCardsNumber % MaxLinesNum == MaxLinesNum / 2 ||
				AllCardsNumber % MaxLinesNum == MaxLinesNum / 2 + 1)
			{
				if ((AllCardsNumber + (MaxLinesNum - AllCardsNumber % MaxLinesNum)) / MaxLinesNum
					<= MaxCardsNumInLine)
				{
					SuitableSpawnStyles.Add(1);
					LinesNumberRelatedToSpawnStyles.Add(MaxLinesNum);
				}
			}

			// If the desired number of lines is even.
			if (AllCardsNumber % (MaxLinesNum - 1) == (MaxLinesNum - 1) / 2)
			{
				if ((AllCardsNumber + ((MaxLinesNum - 1) - AllCardsNumber % (MaxLinesNum - 1))) / (MaxLinesNum - 1)
					<= MaxCardsNumInLine)
				{
					SuitableSpawnStyles.Add(1);
					LinesNumberRelatedToSpawnStyles.Add(MaxLinesNum - 1);
				}
			}
		}
		else
		{
			// If the desired number of lines is odd.

			// In this case, we will be satisfied with
			// the remainder of dividing the desired
			// number of lines by 2, equal to any integer
			// close to the real float number obtained
			// after division (that is, if the result of
			// division is 2.5, the remainder equal to 2
			// and 3 will suit us).
			if (AllCardsNumber % (MaxLinesNum - 1) == (MaxLinesNum - 1) / 2 ||
				AllCardsNumber % (MaxLinesNum - 1) == (MaxLinesNum - 1) / 2 + 1)
			{
				if ((AllCardsNumber + ((MaxLinesNum - 1) - AllCardsNumber % (MaxLinesNum - 1))) / (MaxLinesNum - 1)
					<= MaxCardsNumInLine)
				{
					SuitableSpawnStyles.Add(1);
					LinesNumberRelatedToSpawnStyles.Add(MaxLinesNum - 1);
				}
			}

			// If the desired number of lines is even.
			if (AllCardsNumber % MaxLinesNum == MaxLinesNum / 2)
			{
				if ((AllCardsNumber + (MaxLinesNum - AllCardsNumber % MaxLinesNum)) / MaxLinesNum
					<= MaxCardsNumInLine)
				{
					SuitableSpawnStyles.Add(1);
					LinesNumberRelatedToSpawnStyles.Add(MaxLinesNum);
				}
			}
		}
	}
}

void UCPP_SpawnCardsManager::SpawnCardsByRandomMethod(TArray<int32>& SuitableSpawnStyles,
                                                      TArray<int32>& LinesNumberRelatedToSpawnStyles,
                                                      const int32& MaxCardsNumInLine, const int32& MaxLinesNum,
                                                      const float& Scale,
                                                      const float& StepY, const float& StepZ,
                                                      const int32& AllCardsNumber)
{
	if (SuitableSpawnStyles.Num() <= 0)
	{
		int32 Difference, SuitableCardsNum, SuitableLinesNum;
		if (FindNearestLargerCardsNumberWithRectangleSpawnMethod(Difference, SuitableCardsNum,
		                                                         SuitableLinesNum,
		                                                         AllCardsNumber, MaxLinesNum, MaxCardsNumInLine))
		{
			SubtractiveSpawn(Scale, StepY, StepZ,
			                 Difference, SuitableLinesNum,
			                 SuitableCardsNum, AllCardsNumber);
		}
	}
	else
	{
		const int32 SpawnStyleIndex = FMath::RandRange(0, SuitableSpawnStyles.Num() - 1);

		switch (SuitableSpawnStyles[SpawnStyleIndex])
		{
		default:
		case 0:
			RectangleSpawn(Scale, StepY, StepZ,
			               LinesNumberRelatedToSpawnStyles[SpawnStyleIndex], AllCardsNumber);
			break;
		case 1:
			HerringboneSpawn(Scale, StepY, StepZ,
			                 LinesNumberRelatedToSpawnStyles[SpawnStyleIndex], AllCardsNumber);
			break;
		case 2:
			DecreasingHourglassSpawn(Scale, StepY, StepZ,
			                         LinesNumberRelatedToSpawnStyles[SpawnStyleIndex], AllCardsNumber);
			break;
		case 3:
			IncreasingHourglassSpawn(Scale, StepY, StepZ,
			                         LinesNumberRelatedToSpawnStyles[SpawnStyleIndex], AllCardsNumber);
			break;
		}
	}
}

bool UCPP_SpawnCardsManager::FindNearestLargerCardsNumberWithRectangleSpawnMethod(
	int32& Difference, int32& SuitableCardsNum, int32& SuitableLinesNum,
	const int32& AllCardsNumber, const int32& MaxLinesNum, const int32& MaxCardsNumInLine)
{
	int32 SuitableCardsNumber1 = AllCardsNumber;

	while (SuitableCardsNumber1 % MaxLinesNum != 0)
	{
		SuitableCardsNumber1 += 2;
	}

	int32 SuitableCardsNumber2 = AllCardsNumber;

	while (SuitableCardsNumber2 % (MaxLinesNum - 1) != 0)
	{
		SuitableCardsNumber2 += 2;
	}

	if (SuitableCardsNumber1 / MaxLinesNum <= MaxCardsNumInLine)
	{
		if (SuitableCardsNumber2 / (MaxLinesNum - 1) <= MaxCardsNumInLine)
		{
			if (SuitableCardsNumber1 <= SuitableCardsNumber2)
			{
				SuitableCardsNum = SuitableCardsNumber1;
				SuitableLinesNum = MaxLinesNum;
				Difference = SuitableCardsNum - AllCardsNumber;
				return true;
			}

			SuitableCardsNum = SuitableCardsNumber2;
			SuitableLinesNum = MaxLinesNum - 1;
			Difference = SuitableCardsNum - AllCardsNumber;
			return true;
		}

		SuitableCardsNum = SuitableCardsNumber1;
		SuitableLinesNum = MaxLinesNum;
		Difference = SuitableCardsNum - AllCardsNumber;
		return true;
	}

	if (SuitableCardsNumber2 / (MaxLinesNum - 1) <= MaxCardsNumInLine)
	{
		SuitableCardsNum = SuitableCardsNumber2;
		SuitableLinesNum = MaxLinesNum - 1;
		Difference = SuitableCardsNum - AllCardsNumber;
		return true;
	}

	return false;
}

void UCPP_SpawnCardsManager::SubtractiveSpawn(const float& Scale,
                                              const float& StepY, const float& StepZ,
                                              const int32& Difference, const int32& LinesNum,
                                              const int32& CardsNumForRectangle, const int32& AllCardsNumber)
{
	const int32 MaxCardsNumberInLine = CardsNumForRectangle / LinesNum;

	if (Difference == 2 || Difference == 4)
	{
		int32 CardsNumberInFirstLastLine;
		switch (Difference)
		{
		case 2:
		default:
			CardsNumberInFirstLastLine = MaxCardsNumberInLine - 1;
			break;
		case 4:
			CardsNumberInFirstLastLine = MaxCardsNumberInLine - 2;
			break;
		}
		const float FirstLastLineCoordinateY = -(StepY * (CardsNumberInFirstLastLine - 1)) / 2;
		const float StartCoordinateY = -(StepY * (MaxCardsNumberInLine - 1)) / 2;
		const float StartCoordinateZ = StepZ * (LinesNum - 1) / 2;

		int32 CurrentLineNumber = 1;
		int32 CardsNumberInLine;
		float CoordinateZ = StartCoordinateZ;

		for (int32 i = 0; i < AllCardsNumber; i += CardsNumberInLine)
		{
			float CoordinateY;
			if (CurrentLineNumber == 1 || CurrentLineNumber == LinesNum)
			{
				CoordinateY = FirstLastLineCoordinateY;
				CardsNumberInLine = CardsNumberInFirstLastLine;
			}
			else
			{
				CoordinateY = StartCoordinateY;
				CardsNumberInLine = MaxCardsNumberInLine;
			}

			for (int32 j = i; j < i + CardsNumberInLine; j++)
			{
				SpawnCardOnLevel(Scale, CoordinateY, CoordinateZ, j);
				CoordinateY += StepY;
			}
			CoordinateZ -= StepZ;
			CurrentLineNumber++;
		}
	}
	else if (Difference == 6 || Difference == 8)
	{
		// Cards number in first and last lines.
		int32 CardsNumberInFirstLine;
		// Cards number in second and penultimate lines.
		int32 CardsNumberInSecondLine;
		switch (Difference)
		{
		case 6:
		default:
			CardsNumberInFirstLine = MaxCardsNumberInLine - 2;
			CardsNumberInSecondLine = MaxCardsNumberInLine - 1;
			break;
		case 8:
			CardsNumberInFirstLine = MaxCardsNumberInLine - 2;
			CardsNumberInSecondLine = MaxCardsNumberInLine - 2;
		}

		const float StartCoordinateY = -(StepY * (MaxCardsNumberInLine - 1)) / 2;
		const float StartCoordinateZ = StepZ * (LinesNum - 1) / 2;

		int32 CurrentLineNumber = 1;
		int32 CardsNumberInLine;
		float CoordinateZ = StartCoordinateZ;

		for (int32 i = 0; i < AllCardsNumber; i += CardsNumberInLine)
		{
			float CoordinateY;
			if (CurrentLineNumber == 1 || CurrentLineNumber == LinesNum)
			{
				CoordinateY = -(StepY * (CardsNumberInFirstLine - 1)) / 2;
				CardsNumberInLine = CardsNumberInFirstLine;
			}
			else if (CurrentLineNumber == 2 || CurrentLineNumber == LinesNum - 1)
			{
				CoordinateY = -(StepY * (CardsNumberInSecondLine - 1)) / 2;
				CardsNumberInLine = CardsNumberInSecondLine;
			}
			else
			{
				CoordinateY = StartCoordinateY;
				CardsNumberInLine = MaxCardsNumberInLine;
			}

			for (int32 j = i; j < i + CardsNumberInLine; j++)
			{
				SpawnCardOnLevel(Scale, CoordinateY, CoordinateZ, j);
				CoordinateY += StepY;
			}
			CoordinateZ -= StepZ;
			CurrentLineNumber++;
		}
	}
}

void UCPP_SpawnCardsManager::RectangleSpawn(const float& Scale,
                                            const float& StepY, const float& StepZ,
                                            const int32& LinesNumber, const int32& AllCardsNumber)
{
	const int32 CardsNumberInLine = AllCardsNumber / LinesNumber;
	const float StartCoordinateY = -(StepY * (CardsNumberInLine - 1)) / 2;
	const float StartCoordinateZ = StepZ * (LinesNumber - 1) / 2;

	float CoordinateZ = StartCoordinateZ;
	for (int32 i = 0; i < AllCardsNumber; i += CardsNumberInLine)
	{
		float CoordinateY = StartCoordinateY;
		for (int32 j = i; j < i + CardsNumberInLine; j++)
		{
			SpawnCardOnLevel(Scale, CoordinateY, CoordinateZ, j);
			CoordinateY += StepY;
		}
		CoordinateZ -= StepZ;
	}
}

void UCPP_SpawnCardsManager::HerringboneSpawn(const float& Scale,
                                              const float& StepY, const float& StepZ,
                                              const int32& LinesNumber, const int32& AllCardsNumber)
{
	int32 NumberOfCardsInEvenLines;
	int32 NumberOfCardsInOddLines;

	if (AllCardsNumber % LinesNumber <= LinesNumber / 2)
	{
		// If the remainder is closer to a smaller
		// number, which is multiple of LinesNumber,
		// then there will be MORE cards in the EVEN
		// line.
		NumberOfCardsInOddLines = (AllCardsNumber - (AllCardsNumber % LinesNumber)) / LinesNumber;
		NumberOfCardsInEvenLines = NumberOfCardsInOddLines + 1;
	}
	else
	{
		// If the remainder is closer to a greater
		// number, which is multiple of LinesNumber,
		// then there will be MORE cards in the ODD
		// line.
		NumberOfCardsInOddLines = (AllCardsNumber + (LinesNumber - AllCardsNumber % LinesNumber)) / LinesNumber;
		NumberOfCardsInEvenLines = NumberOfCardsInOddLines - 1;
	}

	const float StartCoordinateYInOddLines = -(StepY * (NumberOfCardsInOddLines - 1)) / 2;
	const float StartCoordinateYInEvenLines = -(StepY * (NumberOfCardsInEvenLines - 1)) / 2;
	const float StartCoordinateZ = -(StepZ * (LinesNumber - 1)) / 2;

	float CoordinateZ = StartCoordinateZ;
	int32 CurrentLineNumber = 1;

	int32 CardsNumberInLine;

	for (int32 i = 0; i < AllCardsNumber; i += CardsNumberInLine)
	{
		float CoordinateY;
		if (CurrentLineNumber % 2 == 0)
		{
			CoordinateY = StartCoordinateYInEvenLines;
			CardsNumberInLine = NumberOfCardsInEvenLines;
		}
		else
		{
			CoordinateY = StartCoordinateYInOddLines;
			CardsNumberInLine = NumberOfCardsInOddLines;
		}

		for (int32 j = i; j < i + CardsNumberInLine; j++)
		{
			SpawnCardOnLevel(Scale, CoordinateY, CoordinateZ, j);
			CoordinateY += StepY;
		}
		CoordinateZ += StepZ;
		CurrentLineNumber++;
	}
}

int32 UCPP_SpawnCardsManager::FindMaxCardsNumInLineForDecreasingHourglassSpawn(
	const int32& LinesNumber, const int32& AllCardsNumber)
{
	// How many should we add
	// to create a rectangle
	// with one side equal to
	// LinesNumber.
	int32 ValueToAdd = 0;

	// Number of lines is odd,
	// so, we should add a central
	// value.
	ValueToAdd += LinesNumber / 2;

	for (int32 i = 2; i <= LinesNumber / 2; i++)
	{
		ValueToAdd += (i - 1) * 2;
	}

	const int32 FirstLineCardsNumber = (AllCardsNumber + ValueToAdd) / LinesNumber;

	return FirstLineCardsNumber;
}

void UCPP_SpawnCardsManager::DecreasingHourglassSpawn(const float& Scale,
                                                      const float& StepY, const float& StepZ,
                                                      const int32& LinesNumber, const int32& AllCardsNumber)
{
	int32 CardsNumberInLine = FindMaxCardsNumInLineForDecreasingHourglassSpawn(LinesNumber, AllCardsNumber);
	float StartCoordinateY = -(StepY * (CardsNumberInLine - 1)) / 2;

	const float StartCoordinateZ = StepZ * (LinesNumber - 1) / 2;
	float CoordinateZ = StartCoordinateZ;

	int32 CurrentLineNumber = 1;

	for (int32 i = 0; i < AllCardsNumber; i += CardsNumberInLine)
	{
		if (CurrentLineNumber != 1)
		{
			if (CurrentLineNumber <= LinesNumber / 2 + 1)
			{
				--CardsNumberInLine;
				StartCoordinateY += StepY / 2;
				CoordinateZ -= StepZ;
			}
			else
			{
				++CardsNumberInLine;
				StartCoordinateY -= StepY / 2;
				CoordinateZ -= StepZ;
			}
		}

		float CoordinateY = StartCoordinateY;
		for (int32 j = i; j < i + CardsNumberInLine; j++)
		{
			SpawnCardOnLevel(Scale, CoordinateY, CoordinateZ, j);
			CoordinateY += StepY;
		}

		CurrentLineNumber++;
	}
}

int32 UCPP_SpawnCardsManager::FindMaxCardsNumInLineForIncreasingHourglassSpawn(
	const int32& LinesNumber, const int32& AllCardsNumber)
{
	// How many should we add
	// to create a rectangle
	// with one side equal to
	// LinesNumber.
	int32 ValueToAdd = 0;
	int32 Counter = 1;
	for (int32 i = LinesNumber / 2; i >= 1; i--)
	{
		ValueToAdd += Counter * 2;
		Counter++;
	}

	const int32 CircleLineCardsNumber = (AllCardsNumber + ValueToAdd) / LinesNumber;

	return CircleLineCardsNumber;
}

void UCPP_SpawnCardsManager::IncreasingHourglassSpawn(const float& Scale,
                                                      const float& StepY, const float& StepZ,
                                                      const int32& LinesNumber, const int32& AllCardsNumber)
{
	const int32 CircleLineCardsNumber =
		FindMaxCardsNumInLineForIncreasingHourglassSpawn(LinesNumber, AllCardsNumber);

	// Firstly, here will be stored
	// the number of cards in the
	// first line (or in the last
	// line).
	int32 CardsNumberInLine = CircleLineCardsNumber - (LinesNumber / 2);

	float StartCoordinateY = -(StepY * (CardsNumberInLine - 1)) / 2;

	const float StartCoordinateZ = StepZ * (LinesNumber - 1) / 2;
	float CoordinateZ = StartCoordinateZ;

	int32 CurrentLineNumber = 1;
	for (int32 i = 0; i < AllCardsNumber; i += CardsNumberInLine)
	{
		if (CurrentLineNumber != 1)
		{
			if (CurrentLineNumber <= LinesNumber / 2 + 1)
			{
				++CardsNumberInLine;
				StartCoordinateY -= StepY / 2;
				CoordinateZ -= StepZ;
			}
			else
			{
				--CardsNumberInLine;
				StartCoordinateY += StepY / 2;
				CoordinateZ -= StepZ;
			}
		}

		float CoordinateY = StartCoordinateY;
		for (int32 j = i; j < i + CardsNumberInLine; j++)
		{
			SpawnCardOnLevel(Scale, CoordinateY, CoordinateZ, j);
			CoordinateY += StepY;
		}
		CurrentLineNumber++;
	}
}

void UCPP_SpawnCardsManager::SpawnCardOnLevel(const float& Scale,
                                              const float& CoordinateY,
                                              const float& CoordinateZ,
                                              const int32& Index)
{
	const FTransform Transform(FRotator(0.0f, 180.0f, 0.0f),
	                           FVector(0.0f, CoordinateY, CoordinateZ),
	                           FVector(Scale, Scale, Scale));

	if (GameInstanceRef)
	{
		ACPP_ParentCard* Card = GetWorld()->
			SpawnActorDeferred<ACPP_ParentCard>(
				GameInstanceRef->GetAssetClassBySoftReference(CurrentLevelCardsOrder[Index]),
				FTransform::Identity,
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::
				AlwaysSpawn);

		if (Card)
		{
			Card->SetCardsDelegatesRef(CardsDelegatesRef);
			if (bSeveralCardsStylesAreUsed)
			{
				Card->SetNewCardBack(MI_GeneralCardBack);
			}
			Card->ChangeModeToPreview();

			UGameplayStatics::FinishSpawningActor(Card, Transform);

			GeneratedCards.Emplace(Card);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstanceRef is not valid!"));
	}
}

void UCPP_SpawnCardsManager::StartCloseAllCardsTimer()
{
	GetWorld()->GetTimerManager().SetTimer(CloseCardsTimer,
	                                       this,
	                                       &UCPP_SpawnCardsManager::CloseAllCards,
	                                       TimeToClose,
	                                       false);
}

void UCPP_SpawnCardsManager::CloseAllCards()
{
	for (const auto& Card : GeneratedCards)
	{
		Card->CloseCard();
	}

	if (GetWorld()->GetTimerManager().TimerExists(CloseCardsTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(CloseCardsTimer);
	}
}
