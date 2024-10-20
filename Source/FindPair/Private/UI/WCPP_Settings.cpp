// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/UI/WCPP_Settings.h"
#include "Components/AudioComponent.h"
class UAudioComponent;

#ifndef CPP_PLAYERCONTROLLER_H
#define CPP_PLAYERCONTROLLER_H
#include "FindPair/Classes/GameMode/CPP_PlayerController.h"
#endif
class ACPP_PlayerController;

#ifndef CPP_GAMEUSERSETTINGS_H
#define CPP_GAMEUSERSETTINGS_H
#include "FindPair/Classes/SaveGame/CPP_GameUserSettings.h"
#endif
class UCPP_GameUserSettings;

#include "Kismet/GameplayStatics.h"

UWCPP_Settings::UWCPP_Settings(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer),
                                                                              SoundMix(nullptr),
                                                                              Music_SoundClass(nullptr),
                                                                              SFX_SoundClass(nullptr),
                                                                              SoundManager(nullptr),
                                                                              Music_Volume(1.0f),
                                                                              SFX_Volume(1.0f),
                                                                              bMusicVolumeIsZero(false),
                                                                              SettingsScrollBox(nullptr),
                                                                              PanelsNumberInAudioTrackSwitcher(0),
                                                                              AudioTrackSwitcher(nullptr),
                                                                              LeftArrowAudioTrackButton(nullptr),
                                                                              RightArrowAudioTrackButton(nullptr),
                                                                              Music_Volume_Slider(nullptr),
                                                                              SFX_Volume_Slider(nullptr),
                                                                              GameInstanceRef(nullptr)
{
}

void UWCPP_Settings::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_SoundManager::StaticClass(), OutActors);
	if (OutActors.Num() > 0 && IsValid(OutActors[0]))
	{
		SoundManager = Cast<ACPP_SoundManager>(OutActors[0]);
	}

	GameInstanceRef = GetWorld()->GetGameInstance<UCPP_GameInstance>();

	if (Music_Volume_Slider && SFX_Volume_Slider)
	{
		Music_Volume = UCPP_GameUserSettings::Get_CPP_GameUserSettings()->GetMusicVolume();
		SFX_Volume = UCPP_GameUserSettings::Get_CPP_GameUserSettings()->Get_SFX_Volume();

		if (Music_Volume == 0)
		{
			bMusicVolumeIsZero = true;
		}
		else
		{
			bMusicVolumeIsZero = false;
		}

		Music_Volume_Slider->SetValue(Music_Volume);
		SFX_Volume_Slider->SetValue(SFX_Volume);

		Music_Volume_Slider->OnValueChanged.AddUniqueDynamic(this, &UWCPP_Settings::MusicSliderValueChanged);
		SFX_Volume_Slider->OnValueChanged.AddUniqueDynamic(this, &UWCPP_Settings::SFX_SliderValueChanged);
	}

	if (LeftArrowAudioTrackButton)
	{
		LeftArrowAudioTrackButton->OnClicked.AddUniqueDynamic(
			this, &UWCPP_Settings::LeftArrowAudioTrackButtonOnClicked);
	}

	if (RightArrowAudioTrackButton)
	{
		RightArrowAudioTrackButton->OnClicked.AddUniqueDynamic(
			this, &UWCPP_Settings::RightArrowAudioTrackButtonOnClicked);
	}

	if (AudioTrackSwitcher && IsValid(SoundManager))
	{
		SoundManager->TrackWasSwitchedDelegate.BindUObject(this, &UWCPP_Settings::TrackWasSwitched);

		switch (SoundManager->GetCurrentTrackNumber())
		{
		case 0:
			AudioTrackSwitcher->SetActiveWidgetIndex(0);
			break;
		case 1:
			AudioTrackSwitcher->SetActiveWidgetIndex(1);
			break;
		case 2:
			AudioTrackSwitcher->SetActiveWidgetIndex(2);
			break;
		default:
			AudioTrackSwitcher->SetActiveWidgetIndex(0);
			break;
		}
	}

	PanelsNumberInAudioTrackSwitcher = AudioTrackSwitcher->GetNumWidgets();
}

void UWCPP_Settings::NativeDestruct()
{
	if (Music_Volume_Slider && SFX_Volume_Slider)
	{
		UCPP_GameUserSettings::Get_CPP_GameUserSettings()->SetMusicVolume(Music_Volume);
		UCPP_GameUserSettings::Get_CPP_GameUserSettings()->Set_SFX_Volume(SFX_Volume);

		UCPP_GameUserSettings::Get_CPP_GameUserSettings()->ApplySettings(false);

		Music_Volume_Slider->OnValueChanged.RemoveDynamic(this, &UWCPP_Settings::MusicSliderValueChanged);
		SFX_Volume_Slider->OnValueChanged.RemoveDynamic(this, &UWCPP_Settings::SFX_SliderValueChanged);
	}

	if (LeftArrowAudioTrackButton)
	{
		LeftArrowAudioTrackButton->OnClicked.RemoveDynamic(
			this, &UWCPP_Settings::LeftArrowAudioTrackButtonOnClicked);
	}

	if (RightArrowAudioTrackButton)
	{
		RightArrowAudioTrackButton->OnClicked.RemoveDynamic(
			this, &UWCPP_Settings::RightArrowAudioTrackButtonOnClicked);
	}

	if (IsValid(SoundManager))
	{
		SoundManager->TrackWasSwitchedDelegate.Unbind();
		SoundManager = nullptr;
	}

	GameInstanceRef = nullptr;

	Super::NativeDestruct();
}

void UWCPP_Settings::MusicSliderValueChanged(float Val)
{
	Music_Volume = Val;
	if (Val == 0.0f)
	{
		SoundManager->SetShouldLoopTrackList(false);
		bMusicVolumeIsZero = true;
	}
	else if (bMusicVolumeIsZero)
	{
		if (!SoundManager->GetShouldLoopTrackList() &&
			!GetWorld()->GetTimerManager().TimerExists(TurnOnTrackListLoopingTimer))
		{
			SoundManager->SetShouldLoopTrackList(true);
		}

		UAudioComponent* AudioComponent = SoundManager->GetCurrentAudioComponent();
		if (IsValid(AudioComponent))
		{
			if (!AudioComponent->IsPlaying())
			{
				AudioComponent->Play(0.0f);
			}
		}

		bMusicVolumeIsZero = false;
	}

	if (SoundMix && Music_SoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, Music_SoundClass, Val,
		                                           1.0f, 0.0f, true);
	}
}

void UWCPP_Settings::SFX_SliderValueChanged(float Val)
{
	SFX_Volume = Val;
	if (SoundMix && SFX_SoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SFX_SoundClass, Val,
		                                           1.0f, 0.0f, true);
	}
}

void UWCPP_Settings::TrackWasSwitched(int32 TrackNumber)
{
	if (TrackNumber < PanelsNumberInAudioTrackSwitcher)
	{
		AudioTrackSwitcher->SetActiveWidgetIndex(TrackNumber);
	}
}

void UWCPP_Settings::LeftArrowAudioTrackButtonOnClicked()
{
	if (IsValid(SoundManager))
	{
		SoundManager->SetShouldLoopTrackList(false);
		SoundManager->StopPlayingCurrentTrack();
		SoundManager->StartPlayingPreviousTrack();

		if (GetWorld()->GetTimerManager().TimerExists(TurnOnTrackListLoopingTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(TurnOnTrackListLoopingTimer);
		}
		GetWorld()->GetTimerManager().SetTimer(TurnOnTrackListLoopingTimer,
		                                       this,
		                                       &UWCPP_Settings::TurnOnTrackListLooping,
		                                       1.0f,
		                                       false);
	}
}

void UWCPP_Settings::RightArrowAudioTrackButtonOnClicked()
{
	if (IsValid(SoundManager))
	{
		SoundManager->SetShouldLoopTrackList(false);
		SoundManager->StopPlayingCurrentTrack();
		SoundManager->StartPlayingNextTrack();

		if (GetWorld()->GetTimerManager().TimerExists(TurnOnTrackListLoopingTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(TurnOnTrackListLoopingTimer);
		}
		GetWorld()->GetTimerManager().SetTimer(TurnOnTrackListLoopingTimer,
		                                       this,
		                                       &UWCPP_Settings::TurnOnTrackListLooping,
		                                       1.0f,
		                                       false);
	}
}

void UWCPP_Settings::TurnOnTrackListLooping()
{
	SoundManager->SetShouldLoopTrackList(true);
	if (GetWorld()->GetTimerManager().TimerExists(TurnOnTrackListLoopingTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(TurnOnTrackListLoopingTimer);
	}
}
