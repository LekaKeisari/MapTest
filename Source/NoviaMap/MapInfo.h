// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapInfo.generated.h"

/**
 * 
 */

//Struct for .pgw file data
USTRUCT(BlueprintType)
struct NOVIAMAP_API FMapInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PixelSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NegativePixelSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoordinateX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoordinateY;

};
