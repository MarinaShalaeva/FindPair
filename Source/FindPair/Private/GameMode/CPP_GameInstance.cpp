// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/GameMode/CPP_GameInstance.h"

void UCPP_GameInstance::Init()
{
	Super::Init();

	SaveManager = NewObject<UCPP_SaveManager>();
	if (IsValid(SaveManager))
	{
		SaveManager->LoadOrCreateSaveGameObject();

		LevelWasEndedDelegateHandle = LevelWasEndedDelegate.AddUObject(SaveManager,
		                                                               &UCPP_SaveManager::TryToSavePlayerResult);
		GetSavedPlayerResultsDelegate.BindUObject(SaveManager, &UCPP_SaveManager::GetSavedPlayerResults);

		SaveNewLevelSettingsDelegate.BindUObject(SaveManager, &UCPP_SaveManager::TryToSaveNewLevelSettings);
		GetSavedLevelSettingsDelegate.BindUObject(SaveManager, &UCPP_SaveManager::GetSavedLevelSettings);

		DeleteSaveGameFileDelegate.BindUObject(SaveManager, &UCPP_SaveManager::DeleteSave);
	}
}

void UCPP_GameInstance::BeginDestroy()
{
	if (IsValid(SaveManager))
	{
		LevelWasEndedDelegate.Remove(LevelWasEndedDelegateHandle);
		LevelWasEndedDelegateHandle.Reset();
		GetSavedPlayerResultsDelegate.Unbind();

		SaveNewLevelSettingsDelegate.Unbind();
		GetSavedLevelSettingsDelegate.Unbind();

		DeleteSaveGameFileDelegate.Unbind();
	}
	SaveManager = nullptr;

	Super::BeginDestroy();
}

UCPP_SaveManager* UCPP_GameInstance::GetSaveManager() const
{
	return SaveManager;
}

UClass* UCPP_GameInstance::GetAssetClassBySoftReference(const TSoftClassPtr<AActor>& AssetPointer)
{
	if (AssetPointer.IsNull())
		return nullptr;

	const FSoftObjectPath AssetToLoad = AssetPointer.ToSoftObjectPath();
	return GetClassByAssetLoader(AssetToLoad);
}

UClass* UCPP_GameInstance::GetWidgetClassBySoftReference(const TSoftClassPtr<UUserWidget>& WidgetPointer)
{
	if (WidgetPointer.IsNull())
		return nullptr;

	const FSoftObjectPath AssetToLoad = WidgetPointer.ToSoftObjectPath();
	return GetClassByAssetLoader(AssetToLoad);
}

UClass* UCPP_GameInstance::GetClassByAssetLoader(const FSoftObjectPath& AssetToLoad)
{
	AssetLoader.LoadSynchronous(AssetToLoad);
	return Cast<UClass>(StaticLoadObject(UClass::StaticClass(),
	                                     nullptr,
	                                     *AssetToLoad.ToString()));
}
