#pragma once

UENUM(BlueprintType)
enum class EBoost : uint8
{
	EB_Health UMETA(DisplayName = "Health"),
	EB_Power UMETA(DisplayName = "Power"),
	EB_Energy UMETA(DisplayName = "Energy"),
	EB_Wisdom UMETA(DisplayName = "Wisdom"),
	EB_Intelligence UMETA(DisplayName = "Intelligence"),
	EB_Combat UMETA(DisplayName = "Combat"),
	EB_Agility UMETA(DisplayName = "Agility"),

	EA_MAX UMETA(DisplayName = "DefaultMAX")
};