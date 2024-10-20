// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/UI/WCPP_PlayerResults.h"
#include "Blueprint/WidgetTree.h"

#define LOCTEXT_NAMESPACE "LoctextNamespace"

UWCPP_PlayerResults::UWCPP_PlayerResults(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	PlayerResultsScrollBox(nullptr),
	GameInstanceRef(nullptr)
{
}

void UWCPP_PlayerResults::NativeConstruct()
{
	Super::NativeConstruct();
	
	GameInstanceRef = GetWorld()->GetGameInstance<UCPP_GameInstance>();

	if (WidgetTree && IsValid(GameInstanceRef) && GameInstanceRef->GetSavedPlayerResultsDelegate.IsBound())
	{
		TArray<struct FPlayerResult> Results;
		if (GameInstanceRef->GetSavedPlayerResultsDelegate.Execute(Results))
		{
			if (Results.Num() > 0)
			{
				PlayerResultsScrollBox->ClearChildren();
				uint32 NameIterator = 0;
				
				for (int32 i = Results.Num() - 1; i >= 0; i--)
				{
					FFormatNamedArguments Args;
					Args.Add(TEXT("LevelModeInscription"), LevelModeInscription);
					if (Results[i].LevelMode == ELevelMode::Classic)
					{
						Args.Add(TEXT("LevelMode"), ClassicLevelModeText);
					}
					else if (Results[i].LevelMode == ELevelMode::TimeReduction)
					{
						Args.Add(TEXT("LevelMode"), TimeReductionLevelModeText);
					}
					Args.Add(TEXT("NumberOfPairsInscription"), NumberOfPairsInscription);
					Args.Add(TEXT("NumberOfPairs"), Results[i].NumberOfPairs);
					Args.Add(TEXT("NumberOfClicksInscription"), NumberOfClicksInscription);
					Args.Add(TEXT("NumberOfClicks"), Results[i].NumberOfClicks);
					Args.Add(TEXT("SpentTimeInscription"), SpentTimeInscription);
					Args.Add(TEXT("TimeResult"), Results[i].TimeResultAsText);

					FFormatNamedArguments TextBlockNameArgs;
					TextBlockNameArgs.Add(TEXT("Index"), NameIterator++);
					UTextBlock* TB = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(),
					                                                         FName(*FText::Format(LOCTEXT
						                                                         ("ResultString",
							                                                         "TextBlock_{Index}"),
							                                                         TextBlockNameArgs).ToString()));
					TB->SetText(FText::Format(LOCTEXT
						("ResultString",
						 "{LevelModeInscription}: {LevelMode}\n"
						 "{NumberOfPairsInscription}: {NumberOfPairs}\n"
						 "{NumberOfClicksInscription}: {NumberOfClicks}\n"
						 "{SpentTimeInscription}: {TimeResult}\n"),
					                          Args));
					SetNewTextBlockStyle(TB);
					PlayerResultsScrollBox->AddChild(TB);
				}
			}
		}
	}
}

void UWCPP_PlayerResults::SetNewTextBlockStyle(UTextBlock*& TextBlock) const
{
	TextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(0.0f, 0.074214f, 0.672443f, 1.0f)));
	TextBlock->SetFont(Font);
	TextBlock->SetJustification(ETextJustify::Left);
	TextBlock->SetAutoWrapText(true);
}
