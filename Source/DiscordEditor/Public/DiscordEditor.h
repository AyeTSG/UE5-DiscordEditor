// Copyright AyeTSG 2022.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "DiscordEditorSettings.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDiscordEditor, All, All);

class FDiscordEditorModule : public IModuleInterface
{
private:
	static void* DiscordDLLHandle;

	FTickerDelegate TickDelegate;
	FTSTicker::FDelegateHandle TickDelegateHandle;

public:
	const UDiscordEditorSettings* DiscordEditorSettings = GetDefault<UDiscordEditorSettings>();

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	bool Tick(float DeltaTime);
};
