// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "MapInfo.h"
#include "MapWidget.generated.h"

/**
 * 
 */
UCLASS()
class NOVIAMAP_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> MapImages;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FMapInfo> MapData;

private:
	//UFUNCTION()
	//static TMap<UTexture2D*, AActor*> InitializeMapData();
	UFUNCTION(BlueprintCallable)
	TArray<UTexture2D*> CreateTextureList();

	UFUNCTION(BlueprintCallable)
	TArray<FMapInfo> CreateMapInfoList();

	UFUNCTION(BlueprintCallable)
	void InitializeData();

	UFUNCTION()
	UTexture2D* LoadTexture2DFromFile(const FString& FilePath, bool& IsValid, int32& Width, int32& Height);
};
