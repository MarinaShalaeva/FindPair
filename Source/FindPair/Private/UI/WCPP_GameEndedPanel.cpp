// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/UI/WCPP_GameEndedPanel.h"

#ifndef CPP_STATICWIDGET_H
#define CPP_STATICWIDGET_H
#include "FindPair/Classes/CPP_StaticLibrary.h"
#endif

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UWCPP_GameEndedPanel::NativeConstruct()
{
	Super::NativeConstruct();

	ExitButton->OnClicked.AddUniqueDynamic(this, &UWCPP_GameEndedPanel::ExitButtonOnClicked);
}

void UWCPP_GameEndedPanel::NativeDestruct()
{
	if (IsValid(ExitButton) && ExitButton->OnClicked.IsAlreadyBound(this, &UWCPP_GameEndedPanel::ExitButtonOnClicked))
	{
		ExitButton->OnClicked.RemoveDynamic(this, &UWCPP_GameEndedPanel::ExitButtonOnClicked);
	}
	Super::NativeDestruct();
}

void UWCPP_GameEndedPanel::ExitButtonOnClicked()
{
	const UWorld* World = GetWorld();
	UKismetSystemLibrary::QuitGame(World,
	                               UGameplayStatics::GetPlayerController(World, 0),
	                               EQuitPreference::Quit,
	                               false);
}

void UWCPP_GameEndedPanel::InitInscription(bool bLevelEndedSuccessfully)
{
	if (bLevelEndedSuccessfully)
	{
		GameEndedTextBlock->SetText(GameEndedSuccessfullyInscription);
	}
	else
	{
		GameEndedTextBlock->SetText(GameOverInscription);
	}
}

void UWCPP_GameEndedPanel::SetResultTime(float SpentSeconds)
{
	ResultTextBlock->SetText(UCPP_StaticLibrary::GetTextRepresentationOfSpentSeconds(SpentSeconds));
}

void UWCPP_GameEndedPanel::SetClicksNumber(uint32 ClicksNumber)
{
	NumberOfClicksTextBlock->SetText(FText::AsNumber(ClicksNumber));
}
