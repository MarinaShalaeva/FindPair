// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_SoundManager.generated.h"

/**
 * Delegate for notifying about ending of current playing
 * track.
 */
DECLARE_DELEGATE_OneParam(FTrackWasSwitched,
                          int32 /* Number of track, which is played after switching. */)

/** Class for working with audio in game. */
UCLASS()
class FINDPAIR_API ACPP_SoundManager : public AActor
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_SoundManager();

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
	 * The root component for storing all other components
	 * of the actor.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Components | Root", meta = (AllowPrivateAccess = true))
	USceneComponent* Root;

	/**
	 * Audio Component for working with the first audio
	 * track.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Audio", meta = (AllowPrivateAccess = true))
	UAudioComponent* TrackAudioComponent_0;

	/**
	 * Audio Component for working with the second audio
	 * track.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Audio", meta = (AllowPrivateAccess = true))
	UAudioComponent* TrackAudioComponent_1;

	/**
	 * Audio Component for working with the third audio
	 * track.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Audio", meta = (AllowPrivateAccess = true))
	UAudioComponent* TrackAudioComponent_2;

	/** USoundMix class object reference. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Classes", meta = (AllowPrivateAccess = true))
	USoundMix* SoundMix;

	/**
	 * USoundClass object reference for changing music
	 * settings.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Classes", meta = (AllowPrivateAccess = true))
	USoundClass* Music_SoundClass;

	/**
	 * USoundClass object reference for changing SFX
	 * settings.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Classes", meta = (AllowPrivateAccess = true))
	USoundClass* SFX_SoundClass;

private:
	/**
	 * The current playing (or last played) audio track
	 * ordinal number.
	 */
	UPROPERTY()
	int32 CurrentTrackNumber;

	/**
	 * Should we play the next audio track after stopping
	 * the currently playing track?
	 */
	UPROPERTY()
	bool bShouldLoopTrackList;

public:
	/**
	 * Delegate for notifying about ending of current playing
	 * track.
	 */
	FTrackWasSwitched TrackWasSwitchedDelegate;

protected:
	/**
	 * Function that is called when any music track is
	 * finished playing.
	 */
	UFUNCTION()
	void AudioTrackWasEnded();

public:
	/**
	 * Function for switching current track to the next one
	 * and for starting to play it.
	 */
	UFUNCTION()
	void StartPlayingNextTrack();

	/**
	 * Function for switching current track to the previous
	 * one and for starting to play it.
	 */
	UFUNCTION()
	void StartPlayingPreviousTrack();

	/**
	 * Function for stopping the current playing audio
	 * track.
	 */
	UFUNCTION()
	void StopPlayingCurrentTrack();

	/**
	 * Setter for the bShouldLoopTrackList variable.
	 * @param NewValue Value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetShouldLoopTrackList(bool NewValue);

	/** Getter for the bShouldLoopTrackList variable. */
	UFUNCTION()
	bool GetShouldLoopTrackList() const;

	/**
	 * Setter for the CurrentTrackNumber variable.
	 * @param NewNumber Value to set.
	 */
	UFUNCTION()
	void SetCurrentTrackNumber(int32 NewNumber);

	/** Getter for the CurrentTrackNumber variable. */
	UFUNCTION()
	int32 GetCurrentTrackNumber() const;

	/**
	 * Getter for the Audio Component related to the current
	 * playing track (or to the last played track, if there
	 * are no playing tracks now).
	 */
	UFUNCTION()
	UAudioComponent* GetCurrentAudioComponent() const;
};
