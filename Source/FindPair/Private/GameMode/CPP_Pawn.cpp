// (c) M. A. Shalaeva, 2023

#include "FindPair/Classes/GameMode/CPP_Pawn.h"

ACPP_Pawn::ACPP_Pawn()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}
