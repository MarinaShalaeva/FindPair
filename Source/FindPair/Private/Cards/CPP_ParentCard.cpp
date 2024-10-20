// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/Cards/CPP_ParentCard.h"
#include "Kismet/GameplayStatics.h"

ACPP_ParentCard::ACPP_ParentCard()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	CardStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM Card"));
	CardStaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Image = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Card Image"));
	Image->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Image->SetRelativeTransform(FTransform(FRotator(0.0f, -90.0f, 0.0f),
	                                       FVector(0.16f, 0.f, 0.f),
	                                       FVector(0.18f, 0.18f, 0.18f)));
}

void ACPP_ParentCard::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CardMode = ECardMode::NoneCardMode;

	Image->SetGenerateOverlapEvents(false);
	Image->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACPP_ParentCard::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(CardStaticMesh) && !CardStaticMesh->OnClicked.IsAlreadyBound(
		this, &ACPP_ParentCard::CustomOnClickedOnCard))
	{
		CardStaticMesh->OnClicked.AddUniqueDynamic(this, &ACPP_ParentCard::CustomOnClickedOnCard);
	}
	
	if (IsValid(CurveFloat))
	{
		TimelineProgressDelegate.BindUFunction(this, FName(TEXT("TimelineProgress")));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgressDelegate);
		CurveTimeline.SetLooping(false);

		StartRotation = FRotator(0.0f, 0.0f, 0.0f);
		EndRotation = FRotator(0.0f, 180.0f, 0.0f);
	}
}

void ACPP_ParentCard::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(CardStaticMesh) && CardStaticMesh->OnClicked.IsAlreadyBound(
		this, &ACPP_ParentCard::CustomOnClickedOnCard))
	{
		CardStaticMesh->OnClicked.RemoveDynamic(this, &ACPP_ParentCard::CustomOnClickedOnCard);
	}
	if (TimelineProgressDelegate.IsBound())
	{
		TimelineProgressDelegate.Unbind();
	}

	CardsDelegatesRef = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ACPP_ParentCard::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurveTimeline.IsPlaying() || CurveTimeline.IsReversing())
	{
		CurveTimeline.TickTimeline(DeltaSeconds);
	}
}

void ACPP_ParentCard::SetCardsDelegatesRef(UCPP_CardsDelegates*& NewCardsDelegatesRef)
{
	CardsDelegatesRef = NewCardsDelegatesRef;
}

void ACPP_ParentCard::TimelineProgress(float Value)
{
	const FRotator NewRotation = FMath::Lerp(StartRotation, EndRotation, Value);
	SetActorRotation(NewRotation);
}

int16 ACPP_ParentCard::GetCardId() const
{
	return CardId;
}

void ACPP_ParentCard::CustomOnClickedOnCard(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (CardMode == ECardMode::Closed)
	{
		CardsDelegatesRef->CardWasOpenedByPlayerDelegate.ExecuteIfBound(this);
	}
	else if (CardMode == ECardMode::Opened)
	{
		CardsDelegatesRef->PlayerTriesToCloseCardDelegate.ExecuteIfBound(this);
	}
}

void ACPP_ParentCard::SetNewCardBack(UMaterialInstance*& NewMaterial) const
{
	CardStaticMesh->SetMaterial(1, NewMaterial);
}

void ACPP_ParentCard::OpenCard()
{
	ChangeModeToOpened();
	if (!CurveTimeline.IsPlaying() && !CurveTimeline.IsReversing())
	{
		CurveTimeline.PlayFromStart();
	}
	else
	{
		CurveTimeline.Stop();
		CurveTimeline.Play();
	}

	if (IsValid(CardWasOpenedSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), CardWasOpenedSound, 1, 1, 0);
	}
}

void ACPP_ParentCard::CloseCard()
{
	ChangeModeToClosed();
	if (!CurveTimeline.IsPlaying() && !CurveTimeline.IsReversing())
	{
		CurveTimeline.ReverseFromEnd();
	}
	else
	{
		CurveTimeline.Stop();
		CurveTimeline.Reverse();
	}
}

void ACPP_ParentCard::CardWasFound()
{
	CardMode = ECardMode::WasFound;
}

void ACPP_ParentCard::ChangeModeToPreview()
{
	CardMode = ECardMode::Preview;
}

void ACPP_ParentCard::ChangeModeToOpened()
{
	CardMode = ECardMode::Opened;
}

void ACPP_ParentCard::ChangeModeToClosed()
{
	CardMode = ECardMode::Closed;
}
