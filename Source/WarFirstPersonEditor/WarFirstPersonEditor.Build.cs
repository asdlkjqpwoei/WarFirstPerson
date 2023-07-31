// Personal Copyright
using UnrealBuildTool;
public class WarFirstPersonEditor : ModuleRules
{
    public WarFirstPersonEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.AddRange(new string[]{"WarFirstPersonEditor"});
        PrivateIncludePaths.AddRange(new string[]{});
        PublicDependencyModuleNames.AddRange(new string[]{"Core", "CoreUObject", "Engine", "EditorFramework", "UnrealEd", "PhysicsCore", "GameplayTagsEditor", "GameplayTasksEditor", "WarFirstPerson"});
        PrivateDependencyModuleNames.AddRange(new string[]{});
    }
}