// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/ScrollBox.h"
class UScrollBox;

#include "WCPP_GameInfo.generated.h"

/**
 * Parent widget class for representing a panel this
 * information about development of the «Find a pair»
 * game.
 */
UCLASS(Abstract)
class FINDPAIR_API UWCPP_GameInfo : public UUserWidget
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_GameInfo(const FObjectInitializer& ObjectInitializer);
	
protected:
	/**
	 * Scroll box with information about game's author, used
	 * assets, etc.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* InfoScrollBox;
};
