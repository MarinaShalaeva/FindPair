// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/GameMode/CPP_GameMode.h"
#include "FindPair/Classes/GameMode/CPP_Pawn.h"

#ifndef CPP_GAMESTATE_H
#define CPP_GAMESTATE_H
#include "FindPair/Classes/GameMode/CPP_GameState.h"
#endif
class ACPP_GameState;

#ifndef CPP_PLAYERCONTROLLER_H
#define CPP_PLAYERCONTROLLER_H
#include "FindPair/Classes/GameMode/CPP_PlayerController.h"
#endif
class ACPP_PlayerController;

ACPP_GameMode::ACPP_GameMode()
{
	static ConstructorHelpers::FClassFinder<AGameStateBase> GameStateBPClass(
		TEXT("/Game/Player/BP_GameState"));
	if (GameStateBPClass.Class != nullptr)
	{
		GameStateClass = GameStateBPClass.Class;
	}
	else
	{
		GameStateClass = ACPP_GameState::StaticClass();
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(
		TEXT("/Game/Player/BP_PlayerController"));
	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	else
	{
		PlayerControllerClass = ACPP_PlayerController::StaticClass();
	}

	static ConstructorHelpers::FClassFinder<APawn> PawnBPClass(
		TEXT("/Game/Player/BP_Pawn"));
	if (PawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PawnBPClass.Class;
	}
	else
	{
		DefaultPawnClass = ACPP_Pawn::StaticClass();
	}

	static ConstructorHelpers::FClassFinder<AHUD> HUD_BPClass(
		TEXT("/Game/UI/BP_HUD"));
	if (HUD_BPClass.Class != nullptr)
	{
		HUDClass = HUD_BPClass.Class;
	}
	else
	{
		HUDClass = ACPP_HUD::StaticClass();
	}
}
