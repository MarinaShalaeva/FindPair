// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
class UTextBlock;

#include "WCPP_LoadingScreen.generated.h"

/**
 * Widget class that contains UI elements that
 * should be shown on the screen during loading
 * level.
 */
UCLASS(Abstract)
class FINDPAIR_API UWCPP_LoadingScreen : public UUserWidget
{
	GENERATED_BODY()
	
	/**
	 * Text block for displaying the word "Loading". 
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* LoadingTextBlock;
};
