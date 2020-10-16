// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DreamPlace : ModuleRules
{
	public DreamPlace(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
