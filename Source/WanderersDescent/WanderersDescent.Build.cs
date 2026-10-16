// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WanderersDescent : ModuleRules
{
	public WanderersDescent(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"WanderersDescent",
			"WanderersDescent/Game",
			"WanderersDescent/Variant_Platforming",
			"WanderersDescent/Variant_Platforming/Animation",
			"WanderersDescent/Variant_Combat",
			"WanderersDescent/Variant_Combat/AI",
			"WanderersDescent/Variant_Combat/Animation",
			"WanderersDescent/Variant_Combat/Gameplay",
			"WanderersDescent/Variant_Combat/Interfaces",
			"WanderersDescent/Variant_Combat/UI",
			"WanderersDescent/Variant_SideScrolling",
			"WanderersDescent/Variant_SideScrolling/AI",
			"WanderersDescent/Variant_SideScrolling/Gameplay",
			"WanderersDescent/Variant_SideScrolling/Interfaces",
			"WanderersDescent/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
