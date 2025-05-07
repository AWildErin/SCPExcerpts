#include "UI/SCPDevMenuWidget.h"

#include "GenericPlatform/GenericPlatformOutputDevices.h"
#include "JsonObjectConverter.h"
#include "HAL/FileManager.h"
#include "Logging/StructuredLog.h"
#include "Misc/Paths.h"
#include "Misc/OutputDeviceFile.h"

#include "Core/SCPGameInstance.h"
#include "SCPGameModule.h"
#include "VersionInfo.h"

USCPDevMenuWidget::USCPDevMenuWidget(const FObjectInitializer& ObjectInitializer) {}

bool USCPDevMenuWidget::GenerateReport(FString ReportComment, FString& OutputPath)
{
	FString DateTimeFormatted = FDateTime::Now().ToString(TEXT("%d-%m-%Y_%H-%M-%S"));
	FString OutputPathLocal = FPaths::ProjectSavedDir() / "Reports/Report_" + DateTimeFormatted;

	UE_LOGFMT(LogSCPDev, Warning, "Writing report to {path}", OutputPathLocal);

	FString LevelSeed = "NO SEED FOUND";
	TObjectPtr<USCPGameInstance> GameInstance = Cast<USCPGameInstance>(GetGameInstance());
	if (GameInstance && !GameInstance->GameSeed.IsEmpty())
	{
		LevelSeed = GameInstance->GameSeed;
	}

	TObjectPtr<UWorld> World = GetWorld();
	if (!World)
	{
		UE_LOGFMT(LogSCPDev, Error, "We can't generate a report if we're not in a world..");
		return false;
	}

	FString BugItGoString = "";
	TObjectPtr<APlayerController> PC = World->GetFirstPlayerController();
	if (PC)
	{
		FVector ViewLocation;
		FRotator ViewRotation;
		PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

		if (TObjectPtr<APawn> Pawn = PC->GetPawn())
		{
			ViewLocation = Pawn->GetActorLocation();
		}

		BugItGoString = FString::Printf(TEXT("BugItGo %f %f %f %f %f %f"), ViewLocation.X, ViewLocation.Y, ViewLocation.Z, ViewRotation.Pitch, ViewRotation.Yaw,
										ViewRotation.Roll);
	}

	// Write everything to the output directory
	FSCPReportInfo Info;
	Info.TimeStamp = FDateTime::Now().ToUnixTimestamp();
	Info.Description = ReportComment;
	Info.LevelName = World->GetName();
	Info.LevelSeed = LevelSeed;
	Info.BugItString = BugItGoString;

	Info.BuildCommit = FVersionInfo::CommitShort;
	Info.BuildBranch = FVersionInfo::BranchName;
	Info.BuildTimeUTC = FVersionInfo::BuildTimeUTC;

	// Save everything
	const FString ScreenshotFileName = OutputPathLocal / "Screenshot.png";
	FScreenshotRequest::RequestScreenshot(ScreenshotFileName, false, false);

	FString ReportString;
	if (FJsonObjectConverter::UStructToJsonObjectString(Info, ReportString, 0, 0, 0, nullptr, true))
	{
		const FString FileName = OutputPathLocal / "Report.json";
		bool bSuccessfulSave = FFileHelper::SaveStringToFile(ReportString, *FileName);
		UE_LOGFMT(LogSCPDev, Error, "First Failed to write report string! {retval}", bSuccessfulSave);
		if (bSuccessfulSave == false)
		{
			UE_LOGFMT(LogSCPDev, Error, "Second Failed to write report string! {retval}", bSuccessfulSave);
			return false;
		}
	}
	else
	{
		UE_LOGFMT(LogSCPDev, Error, "Failed to write report string!");
		return false;
	}

	/** @todo This doesn't work for some reason, really not sure why but needs to be fixed before full playtests */
	// Try to write the report, bail if we can't
	//FOutputDevice* LocalLogFileDevice = FGenericPlatformOutputDevices::GetLog();
	//FOutputDeviceFile* LocalLogFileDeviceFile = static_cast<FOutputDeviceFile*>(LocalLogFileDevice);
	//if (LocalLogFileDeviceFile)
	//{
	//	const FString OutFileName = OutputPathLocal / "Game.log";
	//	bool bExists = FPaths::FileExists(LocalLogFileDeviceFile->GetFilename());
	//	auto test = FPaths::ConvertRelativePathToFull(LocalLogFileDeviceFile->GetFilename());
	//
	//	if (!FPlatformFileManager::Get().GetPlatformFile().CopyFile(LocalLogFileDeviceFile->GetFilename(), *OutFileName, EPlatformFileRead::AllowWrite, EPlatformFileWrite::AllowRead))
	//	{
	//		UE_LOGFMT(LogSCPDev, Error, "Failed to get log file!");
	//		return false;
	//	}
	//}
	//else
	//{
	//	UE_LOGFMT(LogSCPDev, Error, "Failed to get log file!");
	//	return false;
	//}

	OutputPath = OutputPathLocal;
	return true;
}
