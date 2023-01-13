#pragma once

UENUM(BlueprintType)
enum class EAction : uint8
{
	EA_Movement UMETA(DisplayName = "Movement"),

	EA_MAX UMETA(DisplayName = "DefaultMAX")
};