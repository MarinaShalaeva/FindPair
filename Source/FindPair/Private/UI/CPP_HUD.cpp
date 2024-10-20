// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/UI/CPP_HUD.h"

#ifndef TABLEROWS_H
#define TABLEROWS_H
#include "FindPair/Classes/AssetPointer/TableRows.h"
#endif
class UDataTable;

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "FindPair/Classes/UI/CPP_StaticWidgetLibrary.h"
#endif

ACPP_HUD::ACPP_HUD() : PlayerControllerRef(nullptr), GameInstanceRef(nullptr),
                       WidgetBlueprintsDataTable(nullptr),
                       Container_Widget(nullptr),
                       MainMenu_Widget(nullptr),
                       Pause_Widget(nullptr),
                       Level_Widget(nullptr),
                       GameEnded_Widget(nullptr),
                       LoadingScreen_Widget(nullptr)
{
}

void ACPP_HUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<ACPP_PlayerController>(PlayerOwner);

	GameInstanceRef = GetWorld()->GetGameInstance<UCPP_GameInstance>();
	if (GameInstanceRef)
	{
		StartGameDelegateHandle = GameInstanceRef->StartGameDelegate.AddUObject(this, &ACPP_HUD::StartLevel);
		LevelWasEndedDelegateHandle = GameInstanceRef->LevelWasEndedDelegate.AddUObject(this, &ACPP_HUD::LevelWasEnded);
		CanShowGeneratedLevelHandle = GameInstanceRef->CanShowGeneratedLevelDelegate.AddUObject(
			this, &ACPP_HUD::DestroyLoadingScreenWidget);
	}

	InitializeContainerWidget();
	InitializeMainMenuWidget();
}

void ACPP_HUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GameInstanceRef)
	{
		GameInstanceRef->StartGameDelegate.Remove(StartGameDelegateHandle);
		GameInstanceRef->LevelWasEndedDelegate.Remove(LevelWasEndedDelegateHandle);
		GameInstanceRef->CanShowGeneratedLevelDelegate.Remove(CanShowGeneratedLevelHandle);
	}
	StartGameDelegateHandle.Reset();
	LevelWasEndedDelegateHandle.Reset();
	CanShowGeneratedLevelHandle.Reset();

	DestroyMainMenuWidget();
	DestroyPauseWidget();
	DestroyEndedGameWidget();
	DestroyLevelWidget();
	DestroyContainerWidget();

	PlayerControllerRef = nullptr;
	GameInstanceRef = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ACPP_HUD::StartLevel(int32& PairsNumber,
                          ELevelMode& LevelModeToUse,
                          TArray<ECardsStyles>& CardsStylesToUse)
{
	InitializeLoadingScreenWidget();
	DestroyMainMenuWidget();
	InitializeLevelWidget();
}

void ACPP_HUD::LevelWasEnded(bool bSuccess, int32 NumberOfPairs, ELevelMode LevelMode,
                             uint32 ClicksNumber, float SpentSeconds)
{
	InitializeEndedGameWidget(ClicksNumber, SpentSeconds, bSuccess);
}

void ACPP_HUD::InitializeContainerWidget()
{
	if (IsValid(PlayerControllerRef) && Container_Widget == nullptr)
	{
		FName Row = FName(TEXT("Container"));
		UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row
			)
		);
		if (Class)
		{
			Container_Widget = CreateWidget<UWCPP_Container>(PlayerControllerRef, Class);
			if (IsValid(Container_Widget))
			{
				Container_Widget->SetFlags(RF_StrongRefOnFrame);
				Container_Widget->AddToPlayerScreen();
			}
		}
	}
}

void ACPP_HUD::DestroyContainerWidget()
{
	if (IsValid(Container_Widget))
	{
		Container_Widget->RemoveFromParent();
		Container_Widget = nullptr;
	}
}

void ACPP_HUD::InitializeMainMenuWidget()
{
	if (IsValid(PlayerControllerRef) && MainMenu_Widget == nullptr)
	{
		FName Row = FName(TEXT("MainMenu"));
		UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row
			)
		);
		if (Class)
		{
			MainMenu_Widget = CreateWidget<UWCPP_MainMenu>(PlayerControllerRef, Class);
			if (IsValid(MainMenu_Widget))
			{
				MainMenu_Widget->SetFlags(RF_StrongRefOnFrame);
				DestroyPauseWidget();
				Container_Widget->MainMenuAndPauseWidgetSizeBox->AddChild(MainMenu_Widget);
			}
		}
	}
}

void ACPP_HUD::DestroyMainMenuWidget()
{
	if (IsValid(MainMenu_Widget))
	{
		MainMenu_Widget->RemoveFromParent();
		MainMenu_Widget = nullptr;
	}
}

void ACPP_HUD::InitializePauseWidget()
{
	if (IsValid(MainMenu_Widget))
		return;

	if (IsValid(PlayerControllerRef) && Pause_Widget == nullptr)
	{
		FName Row = FName(TEXT("Pause"));
		UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row
			)
		);
		if (Class)
		{
			Pause_Widget = CreateWidget<UWCPP_Pause>(PlayerControllerRef, Class);
			if (IsValid(Pause_Widget) && IsValid(Container_Widget))
			{
				Pause_Widget->SetFlags(RF_StrongRefOnFrame);
				Pause_Widget->ReturnToGameButton->OnClicked.AddDynamic(this, &ACPP_HUD::EndPause);
				Pause_Widget->RestartGameButton->OnClicked.AddDynamic(this, &ACPP_HUD::RestartCurrentGame);
				Pause_Widget->GoToMainMenuButton->OnClicked.AddDynamic(this, &ACPP_HUD::ReturnToMainMenu);
				Container_Widget->MainMenuAndPauseWidgetSizeBox->AddChild(Pause_Widget);
			}
		}
	}
}

void ACPP_HUD::DestroyPauseWidget()
{
	if (IsValid(Pause_Widget))
	{
		Pause_Widget->ReturnToGameButton->OnClicked.RemoveDynamic(this, &ACPP_HUD::EndPause);
		Pause_Widget->RestartGameButton->OnClicked.RemoveDynamic(this, &ACPP_HUD::RestartCurrentGame);
		Pause_Widget->GoToMainMenuButton->OnClicked.RemoveDynamic(this, &ACPP_HUD::ReturnToMainMenu);
		Pause_Widget->RemoveFromParent();
		Pause_Widget = nullptr;
	}
}

void ACPP_HUD::InitializeLevelWidget()
{
	if (IsValid(PlayerControllerRef) && Level_Widget == nullptr)
	{
		FName Row = FName(TEXT("Level"));
		UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row
			)
		);
		if (Class)
		{
			Level_Widget = CreateWidget<UWCPP_Level>(PlayerControllerRef, Class);
			if (IsValid(Level_Widget) && IsValid(Container_Widget) && IsValid(PlayerControllerRef->CardsDelegatesRef))
			{
				Level_Widget->SetFlags(RF_StrongRefOnFrame);
				Level_Widget->InitializeCardsDelegatesVariable(PlayerControllerRef->CardsDelegatesRef);
				Level_Widget->PauseButton->OnClicked.AddDynamic(this, &ACPP_HUD::StartPause);
				Container_Widget->LevelWidgetSizeBox->AddChild(Level_Widget);
			}
		}
	}
}

void ACPP_HUD::DestroyLevelWidget()
{
	if (IsValid(Level_Widget))
	{
		Level_Widget->PauseButton->OnClicked.RemoveDynamic(this, &ACPP_HUD::StartPause);
		Level_Widget->RemoveFromParent();
		Level_Widget = nullptr;
	}
}

void ACPP_HUD::InitializeEndedGameWidget(uint32 ClicksNumber, float SpentSeconds, bool bSuccess)
{
	if (IsValid(PlayerControllerRef) && GameEnded_Widget == nullptr)
	{
		FName Row = FName(TEXT("GameEnded"));
		UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row
			)
		);
		if (Class)
		{
			GameEnded_Widget = CreateWidget<UWCPP_GameEndedPanel>(PlayerControllerRef, Class);
			if (IsValid(GameEnded_Widget) && IsValid(Container_Widget))
			{
				GameEnded_Widget->SetFlags(RF_StrongRefOnFrame);
				GameEnded_Widget->InitInscription(bSuccess);
				GameEnded_Widget->SetClicksNumber(ClicksNumber);
				GameEnded_Widget->SetResultTime(SpentSeconds);
				GameEnded_Widget->GoToMainMenuButton->OnClicked.AddDynamic(this, &ACPP_HUD::ReturnToMainMenu);
				GameEnded_Widget->RestartGameButton->OnClicked.AddDynamic(this, &ACPP_HUD::RestartCurrentGame);
				Container_Widget->GameEndedSizeBox->AddChild(GameEnded_Widget);
			}
		}
	}
}

void ACPP_HUD::DestroyEndedGameWidget()
{
	if (IsValid(GameEnded_Widget))
	{
		GameEnded_Widget->GoToMainMenuButton->OnClicked.RemoveDynamic(this, &ACPP_HUD::ReturnToMainMenu);
		GameEnded_Widget->RestartGameButton->OnClicked.RemoveDynamic(this, &ACPP_HUD::RestartCurrentGame);
		GameEnded_Widget->RemoveFromParent();
		GameEnded_Widget = nullptr;
	}
}

void ACPP_HUD::InitializeLoadingScreenWidget()
{
	if (IsValid(PlayerControllerRef) && LoadingScreen_Widget == nullptr)
	{
		FName Row = FName(TEXT("Loading"));
		UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row
			)
		);
		if (Class)
		{
			LoadingScreen_Widget = CreateWidget<UWCPP_LoadingScreen>(PlayerControllerRef, Class);
			if (IsValid(LoadingScreen_Widget) && IsValid(Container_Widget))
			{
				LoadingScreen_Widget->SetFlags(RF_StrongRefOnFrame);
				Container_Widget->LoadingWidgetSizeBox->AddChild(LoadingScreen_Widget);
			}
		}
	}
}

void ACPP_HUD::DestroyLoadingScreenWidget()
{
	if (IsValid(LoadingScreen_Widget))
	{
		LoadingScreen_Widget->RemoveFromParent();
		LoadingScreen_Widget = nullptr;
	}
}

void ACPP_HUD::StartPause()
{
	if (IsValid(MainMenu_Widget) || IsValid(GameEnded_Widget))
		return;

	InitializePauseWidget();
	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->StartPause();
	}
}

void ACPP_HUD::EndPause()
{
	DestroyPauseWidget();
	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->EndPause();
	}
}

void ACPP_HUD::RestartCurrentGame()
{
	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->EndPause();
	}

	InitializeLoadingScreenWidget();
	DestroyEndedGameWidget();
	DestroyPauseWidget();

	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->RestartCurrentGame();
	}
}

void ACPP_HUD::ReturnToMainMenu()
{
	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->ReturnToMainMenu();
	}
	DestroyEndedGameWidget();
	DestroyLevelWidget();
	InitializeMainMenuWidget();
}
