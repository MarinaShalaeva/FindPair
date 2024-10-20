// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/CPP_StaticLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Source: https://nerivec.github.io/old-ue4-wiki/pages/float-as-string-with-precision.html
FText UCPP_StaticLibrary::GetFloatAsTextWithPrecision(float TheFloat,
                                                      int32 Precision,
                                                      bool IncludeLeadingZero = true)
{
	// Round to integral if TheFloat variable has something
	// like 1.9999 within precision.
	const float Rounded = roundf(TheFloat);
	if (FMath::Abs(TheFloat - Rounded) < FMath::Pow(10.0f, -1 * Precision))
	{
		TheFloat = Rounded;
	}
	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	return FText::AsNumber(TheFloat, &NumberFormat);
}

FText UCPP_StaticLibrary::GetTextRepresentationOfSpentSeconds(float SpentSeconds)
{
	TArray<FText> Arr;
	const int32 RoundSeconds = UKismetMathLibrary::Round(SpentSeconds);

	const int32 Hours = RoundSeconds / 3600;
	if (Hours != 0)
	{
		Arr.Emplace(FText::AsNumber(Hours));
		Arr.Emplace(FText::AsCultureInvariant(TEXT(":")));
	}

	const int32 Minutes = (RoundSeconds / 60) - 60 * (RoundSeconds / 3600);

	if (Minutes / 10 == 0)
	{
		Arr.Emplace(FText::AsCultureInvariant(TEXT("0")));
	}
	Arr.Emplace(FText::AsNumber(Minutes));
	Arr.Emplace(FText::AsCultureInvariant(TEXT(":")));

	const int32 Seconds = RoundSeconds % 60;
	if (Seconds / 10 == 0)
	{
		Arr.Emplace(FText::AsCultureInvariant(TEXT("0")));
	}

	Arr.Emplace(FText::AsNumber(FGenericPlatformMath::TruncToInt
		(SpentSeconds - (Minutes * 60 + Hours * 3600))));

	FText WholePart, Reminder;
	GetWholePartAndRemainderAsText(SpentSeconds, 2, WholePart, Reminder);

	Arr.Emplace(FText::AsCultureInvariant(TEXT(",")));
	Arr.Emplace(Reminder);

	return FText::Join(FText::AsCultureInvariant(TEXT("")), Arr);
}

void UCPP_StaticLibrary::GetWholePartAndRemainderAsText(float TheFloat, int32 Precision,
                                                        FText& OutWholePart, FText& OutReminder)
{
	const float Rounded = roundf(TheFloat);
	if (FMath::Abs(TheFloat - Rounded) < FMath::Pow(10.0f, -1 * Precision))
	{
		TheFloat = Rounded;
	}

	const int32 Whole = static_cast<int32>(TheFloat);

	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumFractionalDigits = 0;
	NumberFormat.MaximumFractionalDigits = 0;

	OutWholePart = FText::AsNumber(Whole, &NumberFormat);
	OutReminder = FText::AsNumber((TheFloat - Whole) * 100, &NumberFormat);
}

FText UCPP_StaticLibrary::GetSecondsToDisplayFromSecondsNumber(float Seconds)
{
	const int32 RoundSeconds = UKismetMathLibrary::Round(Seconds);
	const int32 ResultSeconds = RoundSeconds % 60;

	FText FinalText = FText::AsNumber(ResultSeconds);
	if (ResultSeconds / 10 == 0)
	{
		TArray<FText> Arr;
		Arr.Emplace(FText::AsCultureInvariant(TEXT("0")));
		Arr.Emplace(FinalText);
		FinalText = FText::Join(FText::AsCultureInvariant(TEXT("")), Arr);
	}

	return FinalText;
}

FText UCPP_StaticLibrary::GetMinutesToDisplayFromSecondsNumber(float Seconds)
{
	return UCPP_StaticLibrary::GetMinutesToDisplayFromRoundSecondsNumber
		(UKismetMathLibrary::Round(Seconds));
}

FText UCPP_StaticLibrary::GetMinutesToDisplayFromRoundSecondsNumber(int32 RoundSeconds)
{
	const int32 Minutes = (RoundSeconds / 60) - 60 * (RoundSeconds / 3600);

	FText FinalText = FText::AsNumber(Minutes);
	if (Minutes / 10 == 0)
	{
		TArray<FText> Arr;
		Arr.Emplace(FText::AsCultureInvariant(TEXT("0")));
		Arr.Emplace(FinalText);
		FinalText = FText::Join(FText::AsCultureInvariant(TEXT("")), Arr);
	}
	return FinalText;
}
