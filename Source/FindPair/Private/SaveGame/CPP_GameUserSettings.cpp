// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/SaveGame/CPP_GameUserSettings.h"

UCPP_GameUserSettings::UCPP_GameUserSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AudioTrackNumber = 0;
	Music_Volume = 1.0f;
	SFX_Volume = 1.0f;
}

void UCPP_GameUserSettings::SetAudioTrackNumber(int32 NewValue)
{
	AudioTrackNumber = NewValue;
}

int32 UCPP_GameUserSettings::GetAudioTrackNumber() const
{
	return AudioTrackNumber;
}

void UCPP_GameUserSettings::SetMusicVolume(float NewValue)
{
	Music_Volume = NewValue;
}

float UCPP_GameUserSettings::GetMusicVolume() const
{
	return Music_Volume;
}

void UCPP_GameUserSettings::Set_SFX_Volume(float NewValue)
{
	SFX_Volume = NewValue;
}

float UCPP_GameUserSettings::Get_SFX_Volume() const
{
	return SFX_Volume;
}

UCPP_GameUserSettings* UCPP_GameUserSettings::Get_CPP_GameUserSettings()
{
	return Cast<UCPP_GameUserSettings>(UGameUserSettings::GetGameUserSettings());
}
