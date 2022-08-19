// Copyright AyeTSG 2022.

#pragma once

#include "CoreMinimal.h"
#include "DiscordEditorSettings.generated.h"

/**
 * Settings for Discord for Unreal Editor
 */
UCLASS(Config=Editor, DefaultConfig)
class DISCORDEDITOR_API UDiscordEditorSettings : public UObject
{
	GENERATED_BODY()
	
public:
	/** Default constructor */
	UDiscordEditorSettings();
	
	/** Should the Rich Presence show elapsed time? */
	UPROPERTY(Config, EditAnywhere, Category = "Rich Presence|Configuration")
	bool bShowElapsedTime;

	/** Should project / asset names be hidden? */
	UPROPERTY(Config, EditAnywhere, Category = "Rich Presence|Configuration")
	bool bUsePrivacyMode;

	/** Should the editor attempt to use Discord's Rich Presence feature? */
	UPROPERTY(Config, EditAnywhere, Category = "Rich Presence|Configuration")
	bool bUseRichPresence;

	/** The default Client ID to use for Rich Presence */
	UPROPERTY(Config, EditAnywhere, Category = "Rich Presence|Configuration")
	FString ClientID;

	/** The default Large Image key to use */
	UPROPERTY(Config, EditAnywhere, Category = "Rich Presence|Assets|Large Image")
	FString LargeImageKey;

	/** The default Large Image text to use */
	UPROPERTY(Config, EditAnywhere, Category = "Rich Presence|Assets|Large Image")
	FString LargeImageText;
};
