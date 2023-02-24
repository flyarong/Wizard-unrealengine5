#pragma once

UENUM(BlueprintType)
enum class EAction : uint8
{
	EA_Movement UMETA(DisplayName = "Movement"),
	EA_Combat UMETA(DisplayName = "Combat"),

	EA_MAX UMETA(DisplayName = "DefaultMAX")
};