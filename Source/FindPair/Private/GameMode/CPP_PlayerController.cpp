// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/GameMode/CPP_PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#ifndef CPP_GAMESTATE_H
#define CPP_GAMESTATE_H
#include "FindPair/Classes/GameMode/CPP_GameState.h"
#endif
class ACPP_GameState;

ACPP_PlayerController::ACPP_PlayerController() : SoundManager(nullptr),
                                                 CardsDelegatesRef(nullptr),
                                                 HUD_Ref(nullptr)
{
}

void ACPP_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUD_Ref = Cast<ACPP_HUD>(GetHUD());

	ChangeCursorVisibility(true);
}

void ACPP_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	HUD_Ref = nullptr;
	SoundManager = nullptr;
	CardsDelegatesRef = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ACPP_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FInputActionBinding& PauseInputAction = InputComponent->BindAction("Pause", IE_Released, this,
	                                                                   &ACPP_PlayerController::PauseKeyPressed);
	PauseInputAction.bExecuteWhenPaused = true;
}

void ACPP_PlayerController::ChangeCursorVisibility(bool bShouldBeVisible)
{
	this->bShowMouseCursor = bShouldBeVisible;
	this->bEnableClickEvents = bShouldBeVisible;
	this->bEnableMouseOverEvents = bShouldBeVisible;

	if (IsValid(HUD_Ref) && IsValid(HUD_Ref->Container_Widget))
	{
		if (bShouldBeVisible)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this,
			                                                  HUD_Ref->Container_Widget,
			                                                  EMouseLockMode::DoNotLock);
		}
		else
		{
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this,
			                                                  HUD_Ref->Container_Widget,
			                                                  EMouseLockMode::LockAlways);
		}
	}
}

void ACPP_PlayerController::SetCardsDelegatesRef(UCPP_CardsDelegates* NewCardsDelegatesRef)
{
	CardsDelegatesRef = NewCardsDelegatesRef;
}

void ACPP_PlayerController::RestartCurrentGame()
{
	if (IsValid(CardsDelegatesRef))
	{
		if (CardsDelegatesRef->ShouldDestroyAllCardsDelegate.IsBound())
		{
			CardsDelegatesRef->ShouldDestroyAllCardsDelegate.Execute();
		}
		if (CardsDelegatesRef->ShouldRestartLevelDelegate.IsBound())
		{
			CardsDelegatesRef->ShouldRestartLevelDelegate.Execute();
		}
	}
}

void ACPP_PlayerController::ReturnToMainMenu()
{
	EndPause();

	if (IsValid(CardsDelegatesRef) && CardsDelegatesRef->ShouldDestroyAllCardsDelegate.IsBound())
	{
		CardsDelegatesRef->ShouldDestroyAllCardsDelegate.Execute();
	}

	ACPP_GameState* GS = GetWorld()->GetGameState<ACPP_GameState>();
	if (IsValid(GS))
	{
		GS->CallChangingSkyMaterialToDefault();
	}
}

void ACPP_PlayerController::StartPause()
{
	SetPause(true);
}

void ACPP_PlayerController::EndPause()
{
	SetPause(false);
}

void ACPP_PlayerController::PauseKeyPressed()
{
	if (IsPaused())
	{
		EndPause();
	}
	else
	{
		StartPause();
	}
}
