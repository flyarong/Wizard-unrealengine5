#pragma once

UENUM(BlueprintType)
enum class EAttribute : uint8
{
	EA_Health UMETA(DisplayName = "Health"),
	EA_Power UMETA(DisplayName = "Power"),
	EA_Energy UMETA(DisplayName = "Energy"),
	EA_XP UMETA(DisplayName = "XP"),
	EA_Wisdom UMETA(DisplayName = "Wisdom"),
	EA_Intelligence UMETA(DisplayName = "Intelligence"),
	EA_Combat UMETA(DisplayName = "Combat"),
	EA_Agility UMETA(DisplayName = "Agility"),

	EA_MAX UMETA(DisplayName = "DefaultMAX")
};