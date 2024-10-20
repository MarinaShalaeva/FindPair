// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

class UDataTable;
#ifndef TABLEROWS_H
#define TABLEROWS_H
#include "FindPair/Classes/AssetPointer/TableRows.h"
#endif

#ifndef CPP_SPAWNCARDSMANAGER_H
#define CPP_SPAWNCARDSMANAGER_H
#include "FindPair/Classes/Cards/CPP_SpawnCardsManager.h"
#endif
class UCPP_SpawnCardsManager;

#include "CPP_GameState.generated.h"

/** Delegate for changing sky material. */
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeSkyMaterial,
                                             TSoftObjectPtr<UMaterialInstanceConstant>, Material,
                                             FRotator, SkySphereRotation);

/** C++ parent Game State class. */
UCLASS()
class FINDPAIR_API ACPP_GameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_GameState();

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
	 * Delegate handle to an object-function pair bound to
	 * a StartGameDelegate.
	 */
	FDelegateHandle StartGameDelegateHandle;

public:
	/** Delegate (event dispatcher) for changing sky material. */
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
	FChangeSkyMaterial ED_ChangeSkyMaterial;

protected:
	/** Reference to the instance of UCPP_CardsDelegates class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UCPP_CardsDelegates* CardsDelegatesRef;

	/**
	 * Reference to the instance of UCPP_SpawnCardsManager
	 * class.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UCPP_SpawnCardsManager* SpawnCardsManager;

	/**
	 * Material instance of universal card's back, which can
	 * be used if several cards' styles are selected.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UMaterialInstance* MI_GeneralCardBack;

	/**
	 * Data Table with cards' classes related to Halloween
	 * style.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UDataTable* HalloweenCardsDataTable;

	/**
	 * Data Table with cards' classes related to Autumn
	 * style.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UDataTable* AutumnCardsDataTable;

	/**
	 * Data Table with cards' classes related to Winter
	 * style.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UDataTable* WinterCardsDataTable;

	/**
	 * Data Table with cards' classes related to Bakery
	 * style.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UDataTable* BakeryCardsDataTable;

	/**
	 * Data Table with cards' classes related to Sport
	 * style.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UDataTable* SportCardsDataTable;

	/**
	 * Data Table with cards' classes related to Cleaning
	 * style.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UDataTable* CleaningCardsDataTable;

	/**
	 * Data Table with cards' classes related to Covid
	 * style.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UDataTable* CovidCardsDataTable;

	/**
	 * Data Table with cards' classes related to Flowers
	 * style.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UDataTable* FlowersCardsDataTable;

	/**
	 * Data Table with cards' classes related to Spring
	 * style.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UDataTable* SpringCardsDataTable;

	/**
	 * Data Table with cards' classes related to Summer
	 * style.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UDataTable* SummerCardsDataTable;

	/**
	 * Data Table with cards' classes related to Valentines
	 * style.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UDataTable* ValentinesCardsDataTable;

	/**
	 * Data Table with cards' classes related to School
	 * style.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cards")
	UDataTable* SchoolCardsDataTable;

	/**
	 * Data Table with sky materials' classes and related
	 * rotations, that should be applied to the sky sphere
	 * static mesh together.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HDR Materials")
	UDataTable* SkyMaterialsDataTable;

	//======«Find a pair» game rules==============

private:
	/**
	 * Is the card that the player is trying to open the
	 * first one? (There are no other opened cards on the
	 * map)
	 */
	bool bUserOpenedFirstCard;

	/** Reference to the first opened card. */
	UPROPERTY()
	ACPP_ParentCard* FirstViewedCard;

	/**
	 * Is the card that the player is trying to open the
	 * second one? (There is one already opened card on the
	 * map)
	 */
	bool bUserOpenedSecondCard;

	/** Reference to the second opened card. */
	UPROPERTY()
	ACPP_ParentCard* SecondViewedCard;

	/** Timer to close two opened cards. */
	FTimerHandle ShowTwoCardsTimer;

	/**
	 * Number of pairs that should be found by player on
	 * this level.
	 */
	int32 AllPairsNumber;

	/** Number of pairs that were found by player. */
	int32 FoundPairsNumber;

	/**
	 * Number of cards' spins performed during playing this
	 * level.
	 */
	uint32 ClicksNumber;

	/** Current level mode. */
	ELevelMode CurrentLevelMode;

	/**
	 * Timer handle for calling an execution of Should Start
	 * Level Delegate.
	 */
	FTimerHandle TH_CallStartLevelDelegate;
	
	/**
	 * Function for starting the level.
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
	 * Function for opening the card on the map.
	 * @param CardRef Reference to the card to be opened.
	 */
	void UserTriedToOpenCard(ACPP_ParentCard* CardRef);

	/**
	 * Function for closing the card on the map.
	 * @param CardRef Reference to the card to be closed.
	 */
	void UserTriedToCloseCard(ACPP_ParentCard* CardRef);

	/** Resetting of ShowTwoCardsTimer. */
	void ResetShowTwoCardsTimer();

	/**
	 * Function for ending the level.
	 * @param bGameEndedSuccessfully Player won?
	 */
	void LevelWasEnded(bool bGameEndedSuccessfully);

	/** Function for unsuccessfully ended level. */
	void GameOver();

public:
	/**
	 * Function for calling changing sky's material instance
	 * and rotation of its static mesh to default values.
	 */
	UFUNCTION()
	void CallChangingSkyMaterialToDefault();

protected:
	/**
	 * Function for getting an asset pointer (soft reference)
	 * to sky material and a rotation that should be applied
	 * to the sky sphere static mesh by the name of the row
	 * from data table.
	 * @param DataTable Data table with the list of available
	 * sky materials.
	 * @param RowName The name of the row from data table,
	 * by which we should find information.
	 * @param OutReference Soft reference to the sky material.
	 * @param OutSkyStaticMeshRotation Rotation that should
	 * be applied to the sky sphere static mesh with the
	 * associated sky material.
	 */
	UFUNCTION()
	void GetSoftReferenceToSkyMaterialFromDataTableByRowName(UDataTable*& DataTable, FName RowName,
	                                                         TSoftObjectPtr<UMaterialInstanceConstant>&
	                                                         OutReference,
	                                                         FRotator& OutSkyStaticMeshRotation);

	/**
	 * Function for getting the list of cards' classes from
	 * the data table related to the particular cards' style.
	 * @param DataTable Data table with a list of cards'
	 * classes designed in the same style.
	 * @param OutReferences Array with the soft references
	 * to cards' classes from data table.
	 */
	UFUNCTION()
	void GetSoftReferencesToCardsFromDataTable(UDataTable*& DataTable,
	                                           TArray<TSoftClassPtr<ACPP_ParentCard>>& OutReferences);
};
