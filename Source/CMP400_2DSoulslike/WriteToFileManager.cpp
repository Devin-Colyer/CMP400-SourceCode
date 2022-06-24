// Fill out your copyright notice in the Description page of Project Settings.

#include "WriteToFileManager.h"

bool WriteToFileManager::WriteLineToFile(FString LineToWrite, FString Directory, FString FileName, bool AllowOverwrite)
{
	FString Path = Directory + "\\" + FileName;

	if (!AllowOverwrite)
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*Path))
			return false;

	return FFileHelper::SaveStringToFile(LineToWrite, *Path);
}

bool WriteToFileManager::AppendLineToFile(FString LineToWrite, FString Directory, FString FileName)
{
	FString Path = Directory + "\\" + FileName;

	TArray<FString> Txt;
	if (FFileHelper::LoadFileToStringArray(Txt, *Path)) {
		Txt.Add(LineToWrite);
		return FFileHelper::SaveStringArrayToFile(Txt, *Path);
	}
	else {
		return false;
	}
}
