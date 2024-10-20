// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"

#ifndef CPP_SAVEMANAGER_H
#define CPP_SAVEMANAGER_H
#include "FindPair/Classes/SaveGame/CPP_SaveManager.h"
#endif
class UCPP_SaveManager;

#include "CPP_GameInstance.generated.h"

/**
 * Delegate for listening to the start of preparing new level
 * event.
 */
DECLARE_MULTICAST_DELEGATE_ThreeParams(FStartGame,
                                       int32& /*Number of pairs*/,
                                       ELevelMode& /*Chosen level mode*/,
                                       TArray<ECardsStyles>& /*Chosen cards styles*/);

/**
 * Delegate for notifying that level was generated and could
 * be shown to the player.
 */
DECLARE_MULTICAST_DELEGATE(FCanShowGeneratedLevel);

/** Delegate for listening to end level event. */
DECLARE_MULTICAST_DELEGATE_FiveParams(FLevelWasEnded,
                                      bool /*If level was ended successfully*/,
                                      int32 /*Number of pairs*/,
                                      ELevelMode /*Level mode*/,
                                      uint32 /*Clicks number*/,
                                      float /*How many seconds player spent on playing this level.*/);

/**
 * Delegate for getting player's results array from the save
 * file.
 */
DECLARE_DELEGATE_RetVal_OneParam(bool /*Success*/,
                                 FGetSavedPlayerResults,
                                 TArray<struct FPlayerResult>& /*Array of player's results*/);

/**
 * Delegate for notifying that level's settings should be
 * saved to the save file.
 */
DECLARE_DELEGATE_ThreeParams(FSaveNewLevelSettings,
                             int32 /*Last chosen number of pairs*/,
                             ELevelMode /*Last chosen level mode*/,
                             TArray<ECardsStyles> /*Last chosen cards' styles*/);

/** Delegate for getting level's settings from the save file. */
DECLARE_DELEGATE_RetVal_ThreeParams(bool /*Success*/,
                                    FGetSavedLevelSettings,
                                    int32& /*Last saved number of pairs*/,
                                    ELevelMode& /*Last saved level mode*/,
                                    TArray<ECardsStyles>& /*Last saved cards' styles*/);

/**
 * Delegate for notifying that the save file should be
 * deleted.
 */
DECLARE_DELEGATE_RetVal(bool /*Success*/,
                        FDeleteSaveGameFile);

/** C++ parent for Game Instance class. */
UCLASS()
class FINDPAIR_API UCPP_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	/**
	 * Function for storing logic that should be applied
	 * when Game Instance appears in the game world.
	 */
	virtual void Init() override;

	/**
	 * Function for storing logic that should be applied in
	 * the end of the Game Instance's life (but before its
	 * destroying).
	 */
	virtual void BeginDestroy() override;

protected:
	/** Reference to the instance of UCPP_SaveManager class. */
	UPROPERTY()
	UCPP_SaveManager* SaveManager;

public:
	/** Getter for the SaveManager variable. */
	UFUNCTION()
	UCPP_SaveManager* GetSaveManager() const;

	/** Delegate for listening to the start level event. */
	FStartGame StartGameDelegate;

	/**
	 * Delegate handle to an object-function pair bound to
	 * a StartGameDelegate.
	 */
	FDelegateHandle LevelWasEndedDelegateHandle;

	/**
	 * Delegate for notifying that level was generated and
	 * could be shown to the player.
	 */
	FCanShowGeneratedLevel CanShowGeneratedLevelDelegate;

	/** Delegate for listening to end level event. */
	FLevelWasEnded LevelWasEndedDelegate;

	/**
	 * Delegate for getting player's results array from the
	 * save file.
	 */
	FGetSavedPlayerResults GetSavedPlayerResultsDelegate;

	/**
	 * Delegate for notifying that level's settings should
	 * be saved to the save file.
	 */
	FSaveNewLevelSettings SaveNewLevelSettingsDelegate;

	/**
	 * Delegate for getting level's settings from the save
	 * file.
	 */
	FGetSavedLevelSettings GetSavedLevelSettingsDelegate;

	/**
	 * Delegate for notifying that the save file should be
	 * deleted.
	 */
	FDeleteSaveGameFile DeleteSaveGameFileDelegate;

	/** The asset loader for loading assets asynchronously. */
	FStreamableManager AssetLoader;

	/**
	 * Function for synchronous receiving class from the
	 * actor's soft reference.
	 * @param AssetPointer Soft reference to actor class.
	 * @return Class derived from the soft reference.
	 */
	UClass* GetAssetClassBySoftReference(const TSoftClassPtr<AActor>& AssetPointer);

	/**
	 * Function for synchronous receiving class from the
	 * widget blueprint's soft reference.
	 * @param WidgetPointer Soft reference to widget
	 * blueprint.
	 * @return Class derived from the soft reference.
	 */
	UClass* GetWidgetClassBySoftReference(const TSoftClassPtr<UUserWidget>& WidgetPointer);

	/**
	 * Function for synchronous receiving class from any
	 * given soft asset reference by using the asset
	 * loader.
	 * @param AssetToLoad Asset reference.
	 * @return Class derived from the asset reference.
	 */
	UFUNCTION()
	UClass* GetClassByAssetLoader(const FSoftObjectPath& AssetToLoad);
};
