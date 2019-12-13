// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TeamDataTableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTeamDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText TeamName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor TeamColor;
};