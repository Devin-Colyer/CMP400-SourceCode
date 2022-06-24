// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class CMP400_2DSOULSLIKE_API WriteToFileManager
{
public:
    static bool WriteLineToFile(FString LineToWrite, FString Directory, FString FileName, bool AllowOverwrite = true);
    static bool AppendLineToFile(FString LineToWrite, FString Directory, FString FileName);
};
