// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/UI/CPP_StaticWidgetLibrary.h"

void UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(UButton*& ButtonRef, const ESlateVisibility Mode)
{
	if (IsValid(ButtonRef))
	{
		ButtonRef->SetVisibility(Mode);
	}
}

void UCPP_StaticWidgetLibrary::ChangePanelWidgetVisibility(UPanelWidget* PanelRef, ESlateVisibility Mode)
{
	if (IsValid(PanelRef))
	{
		PanelRef->SetVisibility(Mode);
	}
}

void UCPP_StaticWidgetLibrary::ChangeTextBlocksVisibility(UTextBlock*& TextBlockRef, ESlateVisibility Mode)
{
	if (IsValid(TextBlockRef))
	{
		TextBlockRef->SetVisibility(Mode);
	}
}

void UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(UButton*& ButtonRef, const bool bShouldEnable)
{
	if (IsValid(ButtonRef))
	{
		ButtonRef->SetIsEnabled(bShouldEnable);
	}
}

TSoftClassPtr<UUserWidget> UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
	UDataTable*& DataTable,
	FName& Row)
{
	if (!IsValid(DataTable))
		return nullptr;

	const FWidgetBlueprintTableRow* FoundRow = DataTable->FindRow<FWidgetBlueprintTableRow>(Row,
		TEXT(""),
		true);

	if (FoundRow)
	{
		return FoundRow->WidgetClass;
	}
	return nullptr;
}
