// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/SaveGame/CPP_SaveManager.h"
#include "Kismet/GameplayStatics.h"

UCPP_SaveManager::UCPP_SaveManager()
{
	SaveGameRef = nullptr;
	bSaveGameObjectIsDeclared = false;
	SaveFileName = FString(TEXT("SaveFile"));
}

void UCPP_SaveManager::SetSaveGameObject(UCPP_SaveGame* NewSaveGame)
{
	if (IsValid(NewSaveGame))
	{
		SaveGameRef = NewSaveGame;
		bSaveGameObjectIsDeclared = true;
	}
	else
	{
		SaveGameRef = nullptr;
		bSaveGameObjectIsDeclared = false;
	}
}

UCPP_SaveGame* UCPP_SaveManager::GetSaveGameObject() const
{
	return SaveGameRef;
}

UCPP_SaveGame* UCPP_SaveManager::LoadOrCreateSaveGameObject()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveFileName, 0))
	{
		USaveGame* Save = UGameplayStatics::LoadGameFromSlot(SaveFileName, 0);
		UCPP_SaveGame* LoadedSaveGameInstance = Cast<UCPP_SaveGame>(Save);
		if (IsValid(LoadedSaveGameInstance))
		{
			SetSaveGameObject(LoadedSaveGameInstance);
			return SaveGameRef;
		}
	}
	else if (UCPP_SaveGame* SaveGameInstance = Cast<UCPP_SaveGame>(
		UGameplayStatics::CreateSaveGameObject(UCPP_SaveGame::StaticClass())))
	{
		SetSaveGameObject(SaveGameInstance);
		return SaveGameRef;
	}
	return nullptr;
}

void UCPP_SaveManager::TryToSavePlayerResult(bool bSuccess, int32 NumberOfPairs,
                                             ELevelMode LevelMode,
                                             uint32 ClicksNumber, float Seconds)
{
	if (!bSuccess)
		return;

	if (!bSaveGameObjectIsDeclared)
	{
		LoadOrCreateSaveGameObject();
	}

	if (!bSaveGameObjectIsDeclared)
		return;

	GetSaveGameObject()->Results.Emplace(FPlayerResult(NumberOfPairs, LevelMode, ClicksNumber, Seconds,
	                                                   UCPP_StaticLibrary::GetTextRepresentationOfSpentSeconds(
		                                                   Seconds)));
	SaveGameInfoToFile();
}

bool UCPP_SaveManager::GetSavedPlayerResults(TArray<FPlayerResult>& Results)
{
	if (!bSaveGameObjectIsDeclared)
		return false;

	if (GetSaveGameObject()->Results.Num() <= 0)
		return false;

	Results = GetSaveGameObject()->Results;
	return true;
}

void UCPP_SaveManager::TryToSaveNewLevelSettings(int32 NumberOfPairs, ELevelMode LevelMode,
                                                 TArray<ECardsStyles> CardsStyles)
{
	if (!bSaveGameObjectIsDeclared)
	{
		LoadOrCreateSaveGameObject();
	}

	if (!bSaveGameObjectIsDeclared)
		return;

	GetSaveGameObject()->LastNumberOfPairs = NumberOfPairs;
	GetSaveGameObject()->LastLevelMode = LevelMode;
	GetSaveGameObject()->LastChosenCardsStyles = CardsStyles;
	SaveGameInfoToFile();
}

bool UCPP_SaveManager::GetSavedLevelSettings(int32& NumberOfPairs, ELevelMode& LevelMode,
                                             TArray<ECardsStyles>& CardsStyles)
{
	if (!bSaveGameObjectIsDeclared)
		return false;

	NumberOfPairs = GetSaveGameObject()->LastNumberOfPairs;
	LevelMode = GetSaveGameObject()->LastLevelMode;
	CardsStyles = GetSaveGameObject()->LastChosenCardsStyles;
	return true;
}

bool UCPP_SaveManager::SaveGameInfoToFile()
{
	if (bSaveGameObjectIsDeclared)
	{
		if (UGameplayStatics::SaveGameToSlot(GetSaveGameObject(), SaveFileName, 0))
		{
			return true;
		}
		return false;
	}
	return false;
}

bool UCPP_SaveManager::DeleteSave()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveFileName, 0))
	{
		bool bSuccess = UGameplayStatics::DeleteGameInSlot(SaveFileName, 0);

		if (bSuccess)
		{
			SetSaveGameObject(nullptr);
		}

		return bSuccess;
	}
	return false;
}
