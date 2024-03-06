// Fill out your copyright notice in the Description page of Project Settings.

#include "MapWidget.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"
#include "Misc/DefaultValueHelper.h"

TArray<UTexture2D*> UMapWidget::CreateTextureList()
{
	TArray<FString> ImageFileNames;
	FString dir = FPaths::ProjectContentDir() + "/tileset/";
	FString fileExt = TEXT("png");
	IFileManager::Get().FindFiles(ImageFileNames, *dir, *fileExt);

	TArray<UTexture2D*> textures;
	bool valid;
	int32 Width;
	int32 Height;

	for (size_t i = 0; i < ImageFileNames.Num(); i++)
	{
		textures.Add( LoadTexture2DFromFile(dir + ImageFileNames[i], valid, Width, Height));
	}

	return textures;
}

TArray<FMapInfo> UMapWidget::CreateMapInfoList()
{
	TArray<FString> DataFileNames;
	FString dir = FPaths::ProjectContentDir() + "/tileset/";
	FString fileExt = TEXT("pgw");
	IFileManager::Get().FindFiles(DataFileNames, *dir, *fileExt);

	TArray<FMapInfo> mapInfoArray;

	for (size_t i = 0; i < DataFileNames.Num(); i++)
	{
		FString DataString;
		TArray<FString> DataLines;
		FFileHelper::LoadFileToString(DataString, *(dir + DataFileNames[i]));

		int lines = DataString.ParseIntoArray(DataLines, TEXT(" "), true);
		UE_LOG(LogTemp, Warning, TEXT("LoadData %d"), lines);

		FMapInfo mapInfo;
		if (DataLines.Num() >= 5)
		{
			FDefaultValueHelper::ParseFloat(DataLines[0], mapInfo.PixelSize);
			FDefaultValueHelper::ParseFloat(DataLines[1], mapInfo.RotationX);
			FDefaultValueHelper::ParseFloat(DataLines[2], mapInfo.RotationY);
			FDefaultValueHelper::ParseFloat(DataLines[3], mapInfo.NegativePixelSize);
			FDefaultValueHelper::ParseFloat(DataLines[4], mapInfo.CoordinateX);
			FDefaultValueHelper::ParseFloat(DataLines[5], mapInfo.CoordinateY);
		}


		mapInfoArray.Add(mapInfo);
	}
	return mapInfoArray;
}

void UMapWidget::InitializeData()
{
	MapImages = CreateTextureList();
	MapData = CreateMapInfoList();

	//for (size_t i = 0; i < textures.Num(); i++) 
	//{
	//	MapCollection.Add(mapInfos[i], textures[i]);
	//	UE_LOG(LogTemp, Warning, TEXT("Creating Map"));
	//}
}

UTexture2D* UMapWidget::LoadTexture2DFromFile(const FString& FilePath, bool& IsValid, int32& Width, int32& Height)
{
	IsValid = false;
	UTexture2D* LoadedT2D = NULL;

	bool isOK = FilePath.IsEmpty();
	if (!isOK) return LoadedT2D;

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

	//Load From File
	FString FullFilePath = FilePath;
	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath)) return NULL;

	//Create T2D!
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
		const TArrayView64<uint8> UncompressedBGRA;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{
			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

			//Valid?
			if (!LoadedT2D) return NULL;
			//~~~~~~~~~~~~~~

			//Out!
			Width = ImageWrapper->GetWidth();
			Height = ImageWrapper->GetHeight();

			//Copy!
			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();

			
			//Update!
			LoadedT2D->UpdateResource();
		}
	}

	// Success!
	IsValid = true;
	return LoadedT2D;
}