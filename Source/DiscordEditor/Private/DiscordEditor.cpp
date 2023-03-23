// Copyright AyeTSG 2022.

#include "DiscordEditor.h"
#include "Interfaces/IPluginManager.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "discord.h"
#include "Misc/App.h"
#include "Misc/DateTime.h"
#include <inttypes.h>

#define LOCTEXT_NAMESPACE "FDiscordEditorModule"

// Create base discord shit
void *FDiscordEditorModule::DiscordDLLHandle = nullptr;
discord::Core *core{};

DEFINE_LOG_CATEGORY(LogDiscordEditor);

void FDiscordEditorModule::StartupModule()
{
	UE_LOG(LogDiscordEditor, Log, TEXT("DiscordEditor module starting up"));

	// == Create Ticker ==
	TickDelegate = FTickerDelegate::CreateRaw(this, &FDiscordEditorModule::Tick);
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate, 1.0f);

	// == Load Discord DLL ==
	FString DiscordDLLPath = FPaths::Combine(IPluginManager::Get().FindPlugin("DiscordEditor")->GetBaseDir(), TEXT("Binaries"), TEXT("Win64"), TEXT("discord_game_sdk.dll"));

	DiscordDLLHandle = FPlatformProcess::GetDllHandle(*DiscordDLLPath);

	if (DiscordDLLHandle == nullptr)
	{
		UE_LOG(LogDiscordEditor, Fatal, TEXT("Discord DLL could not be loaded, causing a fatal error!"));
	}
	else
	{
		UE_LOG(LogDiscordEditor, Log, TEXT("Discord DLL loaded!"));
	}

	// == Register settings ==
	ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Editor", "Plugins", "Discord for Unreal Editor", INVTEXT("Discord for Unreal Editor"), INVTEXT("Configure the Discord for Unreal Editor plug-in."), GetMutableDefault<UDiscordEditorSettings>());
	}

	UE_LOG(LogDiscordEditor, Log, TEXT("Registered settings \"Editor -> Plugins -> Discord for Unreal Editor\""));

	// == Initiate Discord ==

	// Get config data for rich presence
	bool bUseRichPresence = DiscordEditorSettings->bUseRichPresence;

	if (bUseRichPresence)
	{
		// Get config data for Client ID
		FString ClientID_FString = DiscordEditorSettings->ClientID;
		int64 ClientID = FCString::Atoi64(*ClientID_FString);

		// Create the discord core
		auto result = discord::Core::Create(ClientID, DiscordCreateFlags_Default, &core);

		// Get config data for privacy mode
		bool bUsePrivacyMode = DiscordEditorSettings->bUsePrivacyMode;

		// Set the activity
		discord::Activity activity{};

		FString LargeImageKey = DiscordEditorSettings->LargeImageKey;
		activity.GetAssets().SetLargeImage(TCHAR_TO_ANSI(*LargeImageKey));

		FString LargeImageText = DiscordEditorSettings->LargeImageText;
		activity.GetAssets().SetLargeText(TCHAR_TO_ANSI(*LargeImageText));

		FString EditingString;
		if (bUsePrivacyMode)
		{
			EditingString = FString::Printf(TEXT("Editing a project..."));
		}
		else
		{
			EditingString = FString::Printf(TEXT("Editing %s..."), FApp::GetProjectName());
		}

		bool bShowElapsedTime = DiscordEditorSettings->bShowElapsedTime;

		if (bShowElapsedTime)
		{
			FDateTime CurrentTime = FDateTime::UtcNow();
			activity.GetTimestamps().SetStart(CurrentTime.ToUnixTimestamp());
		}

		activity.SetState(TCHAR_TO_ANSI(*EditingString));

		if (core != nullptr)
		{
			core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
			UE_LOG(LogDiscordEditor, Log, TEXT("Sent a presence update!"));
		}
	}
}

void FDiscordEditorModule::ShutdownModule()
{
	UE_LOG(LogDiscordEditor, Log, TEXT("DiscordEditor module shutting down"));

	// == Unload Discord DLL ==

	// Free the DLL handle
	if (DiscordDLLHandle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(DiscordDLLHandle);
		DiscordDLLHandle = nullptr;
	}

	// == Destroy Ticker ==
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);

	// == Unregister settings ==
	ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings("Editor", "Plugins", "Discord for Unreal Editor");
	}

	UE_LOG(LogDiscordEditor, Log, TEXT("Unregistered settings \"Editor -> Plugins -> Discord for Unreal Editor\""));
}

bool FDiscordEditorModule::Tick(float DeltaTime)
{
	if (core != nullptr)
	{
		::core->RunCallbacks();
	}

	return true;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDiscordEditorModule, DiscordEditor)