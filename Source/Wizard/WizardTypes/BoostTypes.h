#pragma once

UENUM(BlueprintType)
enum class EBoost : uint8
{
	EB_Health UMETA(DisplayName = "Health"),
	EB_Defense UMETA(DisplayName = "Defense"),
	EB_Power UMETA(DisplayName = "Power"),
	EB_XP UMETA(DisplayName = "XP"),
	EB_Wisdom UMETA(DisplayName = "Wisdom"),
	EB_Intelligence UMETA(DisplayName = "Intelligence"),
	EB_Offense UMETA(DisplayName = "Offense"),
	EB_Agility UMETA(DisplayName = "Agility"),

	EA_MAX UMETA(DisplayName = "DefaultMAX")
};