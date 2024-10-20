// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Slider.h"
class USlider;
#include "Components/ScrollBox.h"
class UScrollBox;
#include "Components/Button.h"
class UButton;
#include "Components/WidgetSwitcher.h"
class UWidgetSwitcher;

#ifndef CPP_GAMEINSTANCE_H
#define CPP_GAMEINSTANCE_H
#include "FindPair/Classes/GameMode/CPP_GameInstance.h"
#endif
class UCPP_GameInstance;

#ifndef CPP_SOUNDMANAGER_H
#define CPP_SOUNDMANAGER_H
#include "FindPair/Classes/Sound/CPP_SoundManager.h"
#endif
class ACPP_SoundManager;

#include "WCPP_Settings.generated.h"

/** Parent widget class for the settings panel. */
UCLASS(Abstract)
class FINDPAIR_API UWCPP_Settings : public UUserWidget
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_Settings(const FObjectInitializer& ObjectInitializer);

	/**
	 * Function that stores logic that should be applied
	 * when the widget is created.
	 */
	virtual void NativeConstruct() override;

	/**
	 * Function that stores logic that should be applied
	 * when the widget starts destroying.
	 */
	virtual void NativeDestruct() override;

protected:
	/** USoundMix class object reference. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Classes")
	USoundMix* SoundMix;

	/**
	 * USoundClass object reference for changing music
	 * settings.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Classes")
	USoundClass* Music_SoundClass;

	/**
	 * USoundClass object reference for changing SFX
	 * settings.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Classes")
	USoundClass* SFX_SoundClass;

	/** Reference to the instance of ACPP_SoundManager class. */
	UPROPERTY()
	ACPP_SoundManager* SoundManager;

private:
	/** Volume of all music audio tracks. */
	UPROPERTY()
	float Music_Volume;

	/** Volume of all SFX audio tracks. */
	UPROPERTY()
	float SFX_Volume;

	/** Current music volume equals to zero? */
	bool bMusicVolumeIsZero;

	/** Timer for turning on the track list looping. */
	FTimerHandle TurnOnTrackListLoopingTimer;

protected:
	/** Scroll box for showing all settings. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* SettingsScrollBox;

private:
	/** Number of widgets that contain tracks' names. */
	UPROPERTY()
	int32 PanelsNumberInAudioTrackSwitcher;

protected:
	/** Widget switcher containing all tracks' names. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* AudioTrackSwitcher;

	/**
	 * Button to switch the current track to the previous
	 * one.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* LeftArrowAudioTrackButton;

	/** Button to switch the current track to the next one. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RightArrowAudioTrackButton;

	/** Slider for changing music volume. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* Music_Volume_Slider;

	/** Slider for changing SFX volume.	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* SFX_Volume_Slider;

private:
	/** Reference to the instance of GameInstanceRef class. */
	UPROPERTY()
	UCPP_GameInstance* GameInstanceRef;

	//============Functions=======================

	/**
	 * Function that is called after music volume slider
	 * value was changed.
	 * @param Val New slider value.
	 */
	UFUNCTION()
	void MusicSliderValueChanged(float Val);

	/**
	 * Function that is called after SFX volume slider
	 * value was changed.
	 * @param Val New slider value.
	 */
	UFUNCTION()
	void SFX_SliderValueChanged(float Val);

	/**
	 * Function that is called after changing the current
	 * music track to another one. 
	 * @param TrackNumber New selected track number.
	 */
	void TrackWasSwitched(int32 TrackNumber);

	/**
	 * Function for switching the current track to the
	 * previous one.
	 * Function is called after clicking on Left Arrow Audio
	 * Track Button.
	 */
	UFUNCTION()
	void LeftArrowAudioTrackButtonOnClicked();

	/**
	 * Function for switching the current track to the next
	 * one.
	 * Function is called after clicking on Right Arrow Audio
	 * Track Button.
	 */
	UFUNCTION()
	void RightArrowAudioTrackButtonOnClicked();

	/**
	 * Function for turning on the looping of music tracks'
	 * list.
	 */
	void TurnOnTrackListLooping();
};
