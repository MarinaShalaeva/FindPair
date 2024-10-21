// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#ifndef CPP_SOUNDMANAGER_H
#define CPP_SOUNDMANAGER_H
#include "FindPair/Classes/Sound/CPP_SoundManager.h"
#endif
class ACPP_SoundManager;

#ifndef CPP_HUD_H
#define CPP_HUD_H
#include "FindPair/Classes/UI/CPP_HUD.h"
#endif
class ACPP_HUD;

#include "CPP_PlayerController.generated.h"

/**
 * C++ parent Player Controller class.
 */
UCLASS()
class FINDPAIR_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_PlayerController();

	/**
	 * Function for storing logic that should be applied
	 * when the actor appears in the game world.
	 */
	virtual void BeginPlay() override;

	/**
	 * Function for storing logic that should be applied in
	 * the end of the actor's life (but before its destroying).
	 * @param EndPlayReason Specifies why an actor is being
	 * deleted/removed from a level.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Reference to the instance of ACPP_SoundManager class.
	 * Should be initialized in Blueprint.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Manager")
	ACPP_SoundManager* SoundManager;

public:
	/**
	 * Reference to the instance of UCPP_CardsDelegates
	 * class.
	 */
	UPROPERTY()
	UCPP_CardsDelegates* CardsDelegatesRef;

private:
	/** Reference to the instance of ACPP_HUD class. */
	UPROPERTY()
	ACPP_HUD* HUD_Ref;

	/** Called to bind functionality to input. */
	virtual void SetupInputComponent() override;

	/**
	 * Changing cursor's visibility and switching on (or off)
	 * some mouse events, such as Click or Mouse Over events.
	 * @param bShouldBeVisible Should we make cursor visible
	 * and active or not?
	 */
	void ChangeCursorVisibility(bool bShouldBeVisible);

public:
	/**
	 * Setter for the CardsDelegatesRef variable.
	 * @param NewCardsDelegatesRef Value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetCardsDelegatesRef(UCPP_CardsDelegates* NewCardsDelegatesRef);

	/**
	 * Function for restarting the game again with current
	 * settings (with the same number of pairs and selected
	 * cards' styles).
	 */
	UFUNCTION()
	void RestartEndedGame() const;
	
	/**
	 * Function for ending the current find pairs game and
	 * restarting the game again with current settings (with
	 * the same number of pairs and selected cards' styles).
	 */
	UFUNCTION()
	void RestartCurrentGame() const;

	/**
	 * Function for ending current find pairs game and
	 * loading the Main Menu.
	 */
	UFUNCTION()
	void ReturnToMainMenu();

	/** Function for pausing the game. */
	UFUNCTION()
	void StartPause();

	/** Function that takes the game off pause. */
	UFUNCTION()
	void EndPause();

	/** Binding of Pause Key input. */
	UFUNCTION()
	void PauseKeyPressed();
};
