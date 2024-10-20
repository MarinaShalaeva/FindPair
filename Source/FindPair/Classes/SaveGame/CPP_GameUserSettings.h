// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CPP_GameUserSettings.generated.h"

/**
 * The child of UGameUserSettings for storing settings, that
 * are needed in «Find a pair» game.
 */
UCLASS()
class FINDPAIR_API UCPP_GameUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()

protected:
	/**
	 * The ordinal number of the current (or last played)
	 * audio track.
	 */
	UPROPERTY(Config)
	int32 AudioTrackNumber;

	/** Music volume. */
	UPROPERTY(Config)
	float Music_Volume;

	/** Sound effects volume. */
	UPROPERTY(Config)
	float SFX_Volume;

public:
	/**
	 * Setter for the AudioTrackNumber variable.
	 * @param NewValue Value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetAudioTrackNumber(int32 NewValue);

	/** Getter for the AudioTrackNumber variable. */
	UFUNCTION(BlueprintPure)
	int32 GetAudioTrackNumber() const;

	/**
	 * Setter for the Music_Volume variable.
	 * @param NewValue Value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetMusicVolume(float NewValue);

	/** Getter for the Music_Volume variable. */
	UFUNCTION(BlueprintPure)
	float GetMusicVolume() const;

	/**
	 * Setter for the SFX_Volume variable.
	 * @param NewValue Value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void Set_SFX_Volume(float NewValue);

	/** Getter for the SFX_Volume variable. */
	UFUNCTION(BlueprintPure)
	float Get_SFX_Volume() const;

	/**
	 * Getter for the instance of UGameUserSettings class
	 * that was cast to UCPP_GameUserSettings child class.
	 */
	UFUNCTION(BlueprintCallable)
	static UCPP_GameUserSettings* Get_CPP_GameUserSettings();
};
