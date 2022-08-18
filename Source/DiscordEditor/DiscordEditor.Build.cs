// Copyright AyeTSG 2022.

using UnrealBuildTool;
using System.IO;

public class DiscordEditor : ModuleRules
{
	public DiscordEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        // == Discord RPC SDK ==
        PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "discord-files")));
        PublicAdditionalLibraries.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Binaries/Win64/discord_game_sdk.dll.lib")));
		PublicDelayLoadDLLs.Add("discord_game_sdk.dll");
		RuntimeDependencies.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Binaries/Win64/discord_game_sdk.dll")));

		PublicIncludePaths.AddRange(new string[] {  });
		PrivateIncludePaths.AddRange(new string[] {  });	
		
		PublicDependencyModuleNames.AddRange(new string[]{ "Core", "Projects" });
		PrivateDependencyModuleNames.AddRange(new string[]{ "CoreUObject", "Engine", "Slate", "SlateCore" });
		
		DynamicallyLoadedModuleNames.AddRange(new string[]{  });
	}
}
