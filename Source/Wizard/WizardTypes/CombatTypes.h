#pragma once

UENUM(BlueprintType)
enum class ECombat : uint8
{
	EC_GoodSpell UMETA(DisplayName = "Good Spell"),
	EC_DarkSpell UMETA(DisplayName = "Dark Spell"),
	EC_Trial UMETA(DisplayName = "Trial"),
	EC_Enemy UMETA(DisplayName = "Enemy"),

	EC_MAX UMETA(DisplayName = "DefaultMAX")
};