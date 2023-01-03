// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionRogueLike : ModuleRules
{
	public ActionRogueLike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		// Unresolved external symbol error: add the module, in our case was GameplayTasks 
		// GameplayTasks (without the I at the begenning) added for SBTService_CheckAttackRange(The tick function) and AI modules
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","AIModule", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
