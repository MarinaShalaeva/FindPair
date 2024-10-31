// (c) M. A. Shalaeva, 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "Materials/MaterialInstanceConstant.h"
class UMaterialInstanceConstant;

#ifndef CPP_PARENTCARD_H
#define CPP_PARENTCARD_H
#include "FindPair/Classes/Cards/CPP_ParentCard.h"
#endif
class ACPP_ParentCard;

#include "TableRows.generated.h"

/**
 * The table row for storing soft references to cards'
 * blueprints.
 */
USTRUCT(Blueprintable)
struct FCardTableRow : public FTableRowBase
{
	GENERATED_BODY()

	/**
	 * The sequential number of the card within the same
	 * table (of the same style).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CardId;

	/** Soft reference to related card's blueprint class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<ACPP_ParentCard> CardClass;

	FCardTableRow() : CardId(0), CardClass(nullptr)
	{
	}
};

/**
 * The table row for storing soft references to sky
 * materials'.
 */
USTRUCT(Blueprintable)
struct FSkyMaterialTableRow : public FTableRowBase
{
	GENERATED_BODY()

	/** The sequential number of the sky material. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AssetId;

	/**
	 * Rotation for the sky static mesh, which should be
	 * applied together with the material instance of
	 * SkyMaterialClass.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RelatedSkySphereRotation;

	/** Soft reference to related sky material class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInstanceConstant> SkyMaterialClass;

	FSkyMaterialTableRow() : AssetId(0),
	                         RelatedSkySphereRotation(FRotator(0.0f)),
	                         SkyMaterialClass(nullptr)
	{
	}
};

/**
 * The table row for storing soft references to widget
 * blueprints'.
 */
USTRUCT(Blueprintable)
struct FWidgetBlueprintTableRow : public FTableRowBase
{
	GENERATED_BODY()

	/** Soft reference to widget blueprint class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UUserWidget> WidgetClass;

	FWidgetBlueprintTableRow() : WidgetClass(nullptr)
	{
	}
};

/** Container class for table row structs. */
UCLASS()
class FINDPAIR_API UTableRows : public UObject
{
	GENERATED_BODY()
};
