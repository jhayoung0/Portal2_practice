// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class Portal2_practice : ModuleRules
{
	public Portal2_practice(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "CoffeeLibrary" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Platform/Public, Player/Public 폴더를 include 경로에 추가
		PublicIncludePaths.AddRange(new string[] {
			Path.Combine(ModuleDirectory, "Platform", "Public"),
			Path.Combine(ModuleDirectory, "Common", "Public"),
		});
		
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
