// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "PaperSpriteComponent.h"

class UTexture2D;
class UBoxComponent;
class UWidgetComponent;
class UMaterialInstance;

#ifndef CPP_CARDSDELEGATES_H
#define CPP_CARDSDELEGATES_H
#include "FindPair/Classes/Cards/CPP_CardsDelegates.h"
#endif
class UCPP_CardsDelegates;

#include "CPP_ParentCard.generated.h"

/**
 * Enumeration for storing current card's state.
 */
UENUM()
enum class ECardMode : uint8
{
	/** The default value. */
	NoneCardMode,
	/** Card is in the preview mode. */
	Preview,
	/** Card is closed now. */
	Closed,
	/** Card was opened by the player. */
	Opened,
	/** Card was found by the player and is opened now. */
	WasFound
};

/**
 * Class containing the logic of how one card works for the
 * «Find a pair» game.
 */
UCLASS()
class FINDPAIR_API ACPP_ParentCard : public AActor
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	ACPP_ParentCard();

protected:
	/**
	 * Function for specifying some variables after all the
	 * actor's components have been initialized.
	 */
	virtual void PostInitializeComponents() override;

	/**
	 * Function for storing logic that should be applied when
	 * the actor appears in the game world.
	 */
	virtual void BeginPlay() override;

	/**
	 * Function for storing logic that
	 * should be applied in the end of the actor's life (but
	  before its destroying).
	 * @param EndPlayReason Specifies why an actor is being
	 * deleted/removed from a level.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Function for storing logic that should be applied
	  every frame.
	 */
	virtual void Tick(float DeltaSeconds) override;

	// Scene Components.

	/**
	 * The root component for storing all other components
	 * of the actor.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Card Actor | Components", meta = (AllowPrivateAccess = true))
	USceneComponent* Root;

	/** The static mesh component for the card. */
	UPROPERTY(VisibleAnywhere, Category = "Card Actor | Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* CardStaticMesh;

	/**
	 * The paper sprite component for storing the card's
	 * image.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Card Actor | Components",
		meta = (AllowPrivateAccess = true))
	UPaperSpriteComponent* Image;

private:
	/**
	 * Reference to the instance of UCPP_CardsDelegates
	 * class.
	 */
	UPROPERTY()
	UCPP_CardsDelegates* CardsDelegatesRef;

protected:
	// Card info variables.

	/** ID to distinguish one card from another. */
	UPROPERTY(EditAnywhere, Category = "Card Actor | Card Info")
	int16 CardId;

	/**
	 * Reference to sound that should be played when user
	 * opens the card.
	 */
	UPROPERTY(EditAnywhere, Category = "Card Actor | Sound")
	USoundBase* CardWasOpenedSound;

private:
	/** Current card's state. */
	UPROPERTY()
	ECardMode CardMode;

protected:
	/**
	 * Delegate for playing animation of opening and closing
	 * the card.
	 */
	FOnTimelineFloat TimelineProgressDelegate;

	/**
	 * Timeline for playing animation of opening and closing
	 * the card.
	 */
	UPROPERTY()
	FTimeline CurveTimeline;

	/**
	 * Curve for playing animation of opening and closing
	 * the card.
	 */
	UPROPERTY(EditAnywhere, Category = "Card Actor | Timeline")
	UCurveFloat* CurveFloat;

private:
	/**
	 * Start card's rotation for playing animation of opening
	 * and closing the card.
	 */
	FRotator StartRotation;

	/**
	 * End card's rotation for playing animation of opening
	 * and closing the card.
	 */
	FRotator EndRotation;

	// Functions.

	/**
	 * Function for opening or closing card, which will be
	 * called every frame during animation's playing.
	 * @param Value float value for the card's Z-coordinate
	 * interpolation. 
	 */
	UFUNCTION()
	void TimelineProgress(float Value);

public:
	/**
	 * Setter for the CardsDelegatesRef variable.
	 * @param NewCardsDelegatesRef Value to set.
	 */
	UFUNCTION()
	void SetCardsDelegatesRef(UCPP_CardsDelegates*& NewCardsDelegatesRef);

	/** Getter for the CardId variable. */
	int16 GetCardId() const;

	/**
	 * Function that response to clicking on card's static
	 * mesh.
	 * @param TouchedComponent Touched component.
	 * @param ButtonPressed Button that was pressed.
	 */
	UFUNCTION()
	void CustomOnClickedOnCard(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	/**
	 * Function for setting to material instance to card's
	 * back side.
	 * @param NewMaterial Material Instance to set.
	 */
	UFUNCTION()
	void SetNewCardBack(UMaterialInstance*& NewMaterial) const;

	/**
	 * Function that calls changing of the card's state to
	 * «Opened» and for launches the opening animation. 
	 */
	void OpenCard();
	/**
	 * Function that calls changing of the card's state to
	 * «Closed» and launches the closing animation. 
	 */
	void CloseCard();
	/** Function for changing card's state to «Was Found». */
	void CardWasFound();
	/** Function for changing card's state to «Preview». */
	void ChangeModeToPreview();
	/** Function for changing card's state to «Opened». */
	void ChangeModeToOpened();
	/** Function for changing card's state to «Closed». */
	void ChangeModeToClosed();
};
