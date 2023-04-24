#pragma once

UENUM(BlueprintType)
enum class EOutfit : uint8
{
	EO_None UMETA(DisplayName = "None"),
	EO_Gauntlet UMETA(DisplayName = "Gauntlet"),
	EO_Boot UMETA(DisplayName = "Boot"),
	EO_Belt UMETA(DisplayName = "Belt"),
	EO_Armor UMETA(DisplayName = "Armor"),
	EO_Cloak UMETA(DisplayName = "Cloak"),
	EO_Staff UMETA(DisplayName = "Staff"),

	EA_MAX UMETA(DisplayName = "DefaultMAX")
};