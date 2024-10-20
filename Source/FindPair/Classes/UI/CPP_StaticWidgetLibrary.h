// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Components/Button.h"
class UButton;
#include "Components/TextBlock.h"
class UTextBlock;
#include "Components/PanelWidget.h"
class UPanelWidget;

#ifndef TABLEROWS_H
#define TABLEROWS_H
#include "FindPair/Classes/AssetPointer/TableRows.h"
#endif
class UDataTable;

#include "CPP_StaticWidgetLibrary.generated.h"

/** Library with static functions for working with widgets. */
UCLASS()
class FINDPAIR_API UCPP_StaticWidgetLibrary : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Function for changing button's visibility.
	 * @param ButtonRef Reference to button.
	 * @param Mode Visibility mode to set.
	 */
	static void ChangeButtonsVisibility(UButton*& ButtonRef, ESlateVisibility Mode);

	/**
	 * Function for changing visibility of any UPanelWidget's
	 * children (such as horizontal/vertical box).
	 * @param PanelRef Reference to the panel widget.
	 * @param Mode Visibility mode to set.
	 */
	static void ChangePanelWidgetVisibility(UPanelWidget* PanelRef, ESlateVisibility Mode);

	/**
	 * Function for changing text block's visibility.
	 * @param TextBlockRef Reference to text block.
	 * @param Mode Visibility mode to set.
	 */
	static void ChangeTextBlocksVisibility(UTextBlock*& TextBlockRef, ESlateVisibility Mode);

	/**
	 * Function for changing button's enabling.
	 * @param ButtonRef Reference to button.
	 * @param bShouldEnable Should we make a button enabled
	 * or disabled?
	 */
	static void ChangeButtonsEnabling(UButton*& ButtonRef, bool bShouldEnable);

	/**
	 * Function for getting a pointer (soft reference) to
	 * widget blueprint from data table by the name of the
	 * row.
	 * @param DataTable Data table with the list of available
	 * widget blueprints.
	 * @param Row The name of the row from data table, by
	 * which we should find information.
	 * @return Soft reference to the widget blueprint.
	 */
	static TSoftClassPtr<UUserWidget> GetSoftReferenceToWidgetBlueprintByRowName(
		UDataTable*& DataTable,
		FName& Row);
};
