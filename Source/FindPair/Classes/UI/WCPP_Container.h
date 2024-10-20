// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/CanvasPanel.h"
class UCanvasPanel;
#include "Components/SizeBox.h"
class USizeBox;

#include "WCPP_Container.generated.h"

/** Widget container for all other widgets in game. */
UCLASS(Abstract)
class FINDPAIR_API UWCPP_Container : public UUserWidget
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_Container(const FObjectInitializer& ObjectInitializer);
	
protected:
	/** General canvas panel for storing other widgets. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* ContainerCanvasPanel;

public:
	/** Size box for the loading widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* LoadingWidgetSizeBox;

	/** Size box for the main menu and the pause widgets. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* MainMenuAndPauseWidgetSizeBox;

	/** Size box for the level widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* LevelWidgetSizeBox;

	/** Size box for the game ended widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* GameEndedSizeBox;
};
