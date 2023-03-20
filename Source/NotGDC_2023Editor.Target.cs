// Copyright 2023 Nicholas Helish. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class NotGDC_2023EditorTarget : TargetRules
{
	public NotGDC_2023EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("NotGDC_2023");
	}
}
