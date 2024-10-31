// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPP_GameMode.generated.h"

/**
 * C++ Game Mode class for storing information about base
 * classes that will be possessed in world.
 */
UCLASS()
class FINDPAIR_API ACPP_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	/**
	 * The constructor to set base classes that will be
	 * possessed in world.
	 */
	ACPP_GameMode();
};
