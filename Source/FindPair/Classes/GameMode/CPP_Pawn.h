// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Camera/CameraComponent.h"
class UCameraComponent;

#include "CPP_Pawn.generated.h"

/** C++ parent pawn class. */
UCLASS()
class FINDPAIR_API ACPP_Pawn : public APawn
{
	GENERATED_BODY()

public:
	/** The constructor to set default variables. */
	ACPP_Pawn();

	// Scene Components.	
protected:
	/**
	 * The root component for storing all other components
	 * of the actor.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Components | Root", meta = (AllowPrivateAccess = true))
	USceneComponent* Root;

	/** The camera component for storing the user's camera. */
	UPROPERTY(VisibleAnywhere, Category = "Components | Camera", meta = (AllowPrivateAccess = true))
	UCameraComponent* Camera;
};
