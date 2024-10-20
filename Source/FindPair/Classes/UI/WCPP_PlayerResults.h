// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/ScrollBox.h"
class UScrollBox;
#include "Components/TextBlock.h"
class UTextBlock;

#ifndef CPP_GAMEINSTANCE_H
#define CPP_GAMEINSTANCE_H
#include "FindPair/Classes/GameMode/CPP_GameInstance.h"
#endif
class UCPP_GameInstance;

#include "WCPP_PlayerResults.generated.h"

/** Parent widget class for showing player's results. */
UCLASS(Abstract)
class FINDPAIR_API UWCPP_PlayerResults : public UUserWidget
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_PlayerResults(const FObjectInitializer& ObjectInitializer);

protected:
	/**
	 * Function that stores logic that should be applied
	 * when the widget is created.
	 */
	virtual void NativeConstruct() override;

	/** Scroll box for showing all player's results. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* PlayerResultsScrollBox;

	//============Inscriptions====================

	/** Level mode caption. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText LevelModeInscription;
	/** Number of pairs caption. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText NumberOfPairsInscription;
	/** Number of cards' spins caption. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText NumberOfClicksInscription;
	/** Spent on playing level time caption. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText SpentTimeInscription;
	/** Name of the classic level mode. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ClassicLevelModeText;
	/** Name of the time reduction level mode. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText TimeReductionLevelModeText;

	/** Font style for recently created text blocks. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSlateFontInfo Font;

private:
	/** Reference to the instance of GameInstanceRef class. */
	UPROPERTY()
	UCPP_GameInstance* GameInstanceRef;

	/**
	 * Function for updating the style of the text block.
	 * @param TextBlock Reference to text block that should
	 * be modified.
	 */
	UFUNCTION()
	void SetNewTextBlockStyle(UTextBlock*& TextBlock) const;
};
