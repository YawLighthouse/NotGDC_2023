// Copyright 2023 Nicholas Helish. All rights reserved.

using UnrealBuildTool;
using System.IO;

public class NotGDC_2023 : ModuleRules
{
	public NotGDC_2023(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[]
		{
			ModuleDirectory,
			Path.Combine(ModuleDirectory, "GameFramework"),
			Path.Combine(ModuleDirectory, "Presentation"),
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Engine", 
			"HeadMountedDisplay", 
			"EnhancedInput"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"InputCore", 
			"GameplayTags",
			"NetCore",
			"UMG",
			"Slate",
			"SlateCore",
		});
	}
}
