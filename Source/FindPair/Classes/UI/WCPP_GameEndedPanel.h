// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
class UButton;
#include "Components/TextBlock.h"
class UTextBlock;

#include "WCPP_GameEndedPanel.generated.h"

/**
 * Parent widget class for representing a panel that appears
 * at the end of the level.
 */
UCLASS(Abstract)
class FINDPAIR_API UWCPP_GameEndedPanel : public UUserWidget
{
	GENERATED_BODY()

	/**
	 * Function that stores logic that should be applied
	 * when the widget is created.
	 */
	virtual void NativeConstruct() override;

	/**
	 * Function that stores logic that should be applied
	 * when the widget starts destroying.
	 */
	virtual void NativeDestruct() override;

protected:
	/**
	 * Text block for the caption at the top of the widget,
	 * containing information about successful or unsuccessful
	 * level ending.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* GameEndedTextBlock;

	/**
	 * An inscription stating that the player has won at
	 * this level.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText GameEndedSuccessfullyInscription;

	/**
	 * An inscription stating that the player has lost at
	 * this level.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText GameOverInscription;

	/**
	 * Text block related to show number of seconds spent on
	 * playing current level. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ResultTextBlock;

	/**
	 * Text block related to show number of cards' spins
	 * performed during playing current level.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NumberOfClicksTextBlock;

public:
	/** Button to restart level with the same settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* RestartGameButton;

	/** Button to go to the main menu. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* GoToMainMenuButton;

protected:
	/** Button to exit the game. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ExitButton;

private:
	/**
	 * Function intended to exit the game. Function is called
	 * after clicking on Exit Button.
	 */
	UFUNCTION()
	void ExitButtonOnClicked();

public:
	/**
	 * Function intended to set the suitable text for the
	 * caption at the top of the widget.
	 * @param bLevelEndedSuccessfully Did the player win?
	 */
	UFUNCTION()
	void InitInscription(bool bLevelEndedSuccessfully);

	/**
	 * Function intended to set spent on the level seconds
	 * to appropriate text block.
	 * @param SpentSeconds Spent of the level number of
	 * second.
	 */
	UFUNCTION()
	void SetResultTime(float SpentSeconds);

	/**
	 * Function intended to set number of cards' spins
	 * performed during playing the level to appropriate
	 * text block.
	 * @param ClicksNumber Number of cards' spins.
	 */
	UFUNCTION()
	void SetClicksNumber(uint32 ClicksNumber);
};
