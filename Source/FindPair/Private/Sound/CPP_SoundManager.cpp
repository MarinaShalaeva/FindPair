// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/Sound/CPP_SoundManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

#ifndef CPP_GAMEUSERSETTINGS_H
#define CPP_GAMEUSERSETTINGS_H
#include "FindPair/Classes/SaveGame/CPP_GameUserSettings.h"
#endif
class UCPP_GameUserSettings;

ACPP_SoundManager::ACPP_SoundManager()
{
	PrimaryActorTick.bTickEvenWhenPaused = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TrackAudioComponent_0 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent_0"));
	TrackAudioComponent_0->SetupAttachment(RootComponent);

	TrackAudioComponent_1 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent_1"));
	TrackAudioComponent_1->SetupAttachment(RootComponent);

	TrackAudioComponent_2 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent_2"));
	TrackAudioComponent_2->SetupAttachment(RootComponent);

	SoundMix = nullptr;
	Music_SoundClass = nullptr;
	SFX_SoundClass = nullptr;

	bShouldLoopTrackList = false;
	CurrentTrackNumber = 0;
}

void ACPP_SoundManager::BeginPlay()
{
	Super::BeginPlay();

	TrackAudioComponent_0->Stop();
	TrackAudioComponent_1->Stop();
	TrackAudioComponent_2->Stop();

	if (IsValid(UCPP_GameUserSettings::Get_CPP_GameUserSettings()))
	{
		switch (UCPP_GameUserSettings::Get_CPP_GameUserSettings()->GetAudioTrackNumber())
		{
		case 0:
			CurrentTrackNumber = 0;
			TrackAudioComponent_0->Play(0.0f);
			break;
		case 1:
			CurrentTrackNumber = 1;
			TrackAudioComponent_1->Play(0.0f);
			break;
		case 2:
			CurrentTrackNumber = 2;
			TrackAudioComponent_2->Play(0.0f);
			break;
		default:
			CurrentTrackNumber = 0;
			TrackAudioComponent_0->Play(0.0f);
			break;
		}

		if (SoundMix && Music_SoundClass && SFX_SoundClass)
		{
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, Music_SoundClass,
			                                           UCPP_GameUserSettings::Get_CPP_GameUserSettings()->
			                                           GetMusicVolume(),
			                                           1.0f, 0.0f, true);

			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SFX_SoundClass,
			                                           UCPP_GameUserSettings::Get_CPP_GameUserSettings()->
			                                           Get_SFX_Volume(),
			                                           1.0f, 0.0f, true);
		}
	}
	TrackAudioComponent_0->OnAudioFinished.AddUniqueDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	TrackAudioComponent_1->OnAudioFinished.AddUniqueDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	TrackAudioComponent_2->OnAudioFinished.AddUniqueDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
}

void ACPP_SoundManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bShouldLoopTrackList = false;
	if (IsValid(TrackAudioComponent_0))
	{
		TrackAudioComponent_0->OnAudioFinished.RemoveDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	}
	if (IsValid(TrackAudioComponent_1))
	{
		TrackAudioComponent_1->OnAudioFinished.RemoveDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	}
	if (IsValid(TrackAudioComponent_2))
	{
		TrackAudioComponent_2->OnAudioFinished.RemoveDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	}

	Super::EndPlay(EndPlayReason);
}

void ACPP_SoundManager::AudioTrackWasEnded()
{
	if (bShouldLoopTrackList)
	{
		StartPlayingNextTrack();
	}
}

void ACPP_SoundManager::StartPlayingNextTrack()
{
	switch (CurrentTrackNumber)
	{
	case 0:
		SetCurrentTrackNumber(1);
		TrackAudioComponent_1->Play(0.0f);
		TrackWasSwitchedDelegate.ExecuteIfBound(1);
		break;
	case 1:
		SetCurrentTrackNumber(2);
		TrackAudioComponent_2->Play(0.0f);
		TrackWasSwitchedDelegate.ExecuteIfBound(2);
		break;
	case 2:
		SetCurrentTrackNumber(0);
		TrackAudioComponent_0->Play(0.0f);
		TrackWasSwitchedDelegate.ExecuteIfBound(0);
		break;
	default:
		break;
	}
}

void ACPP_SoundManager::StartPlayingPreviousTrack()
{
	switch (CurrentTrackNumber)
	{
	case 0:
		SetCurrentTrackNumber(2);
		TrackAudioComponent_2->Play(0.0f);
		TrackWasSwitchedDelegate.ExecuteIfBound(2);
		break;
	case 1:
		SetCurrentTrackNumber(0);
		TrackAudioComponent_0->Play(0.0f);
		TrackWasSwitchedDelegate.ExecuteIfBound(0);
		break;
	case 2:
		SetCurrentTrackNumber(1);
		TrackAudioComponent_1->Play(0.0f);
		TrackWasSwitchedDelegate.ExecuteIfBound(1);
		break;
	default:
		break;
	}
}

void ACPP_SoundManager::StopPlayingCurrentTrack()
{
	switch (CurrentTrackNumber)
	{
	case 0:
		if (TrackAudioComponent_0->IsPlaying())
		{
			TrackAudioComponent_0->Stop();
		}
		break;
	case 1:
		if (TrackAudioComponent_1->IsPlaying())
		{
			TrackAudioComponent_1->Stop();
		}
		break;
	case 2:
		if (TrackAudioComponent_2->IsPlaying())
		{
			TrackAudioComponent_2->Stop();
		}
		break;
	default:
		if (TrackAudioComponent_0->IsPlaying())
		{
			TrackAudioComponent_0->Stop();
		}
		if (TrackAudioComponent_1->IsPlaying())
		{
			TrackAudioComponent_1->Stop();
		}
		if (TrackAudioComponent_2->IsPlaying())
		{
			TrackAudioComponent_2->Stop();
		}
		break;
	}
}

void ACPP_SoundManager::SetShouldLoopTrackList(bool NewValue)
{
	bShouldLoopTrackList = NewValue;
}

bool ACPP_SoundManager::GetShouldLoopTrackList() const
{
	return bShouldLoopTrackList;
}

void ACPP_SoundManager::SetCurrentTrackNumber(int32 NewNumber)
{
	CurrentTrackNumber = NewNumber;
	if (IsValid(UCPP_GameUserSettings::Get_CPP_GameUserSettings()))
	{
		UCPP_GameUserSettings::Get_CPP_GameUserSettings()->SetAudioTrackNumber(NewNumber);
		UCPP_GameUserSettings::Get_CPP_GameUserSettings()->ApplySettings(false);
	}
}

int32 ACPP_SoundManager::GetCurrentTrackNumber() const
{
	return CurrentTrackNumber;
}

UAudioComponent* ACPP_SoundManager::GetCurrentAudioComponent() const
{
	switch (CurrentTrackNumber)
	{
	case 0:
		return TrackAudioComponent_0;
		break;
	case 1:
		return TrackAudioComponent_1;
		break;
	case 2:
		return TrackAudioComponent_2;
		break;
	default:
		break;
	}
	return nullptr;
}
