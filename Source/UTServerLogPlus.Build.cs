
namespace UnrealBuildTool.Rules
{
	public class UTServerLogPlus : ModuleRules
	{
		public UTServerLogPlus(TargetInfo Target)
        {
            PrivateIncludePaths.Add("UTServerLogPlus/Private");

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
                    "Engine",
                    "UnrealTournament",
				}
			);
		}
	}
}