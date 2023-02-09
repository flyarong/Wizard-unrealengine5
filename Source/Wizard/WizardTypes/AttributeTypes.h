#pragma once

UENUM(BlueprintType)
enum class EAttribute : uint8
{
	EA_Health UMETA(DisplayName = "Health"),
	EA_Defense UMETA(DisplayName = "Defense"),
	EA_Power UMETA(DisplayName = "Power"),
	EA_XP UMETA(DisplayName = "XP"),
	EA_Wisdom UMETA(DisplayName = "Wisdom"),
	EA_Intelligence UMETA(DisplayName = "Intelligence"),
	EA_Offense UMETA(DisplayName = "Offense"),
	EA_Agility UMETA(DisplayName = "Agility"),

	EA_MAX UMETA(DisplayName = "DefaultMAX")
};