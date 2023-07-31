// Personal Copyright

using UnrealBuildTool;

public class WarFirstPerson : ModuleRules
{
	public WarFirstPerson(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "CommonInput", "Core", "CoreUObject", "DeveloperSettings","Engine", "EnhancedInput", "GameplayTags", "InputCore", "Niagara"});

		PrivateDependencyModuleNames.AddRange(new string[] { "CommonUI", "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
