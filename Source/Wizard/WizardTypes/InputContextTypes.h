#pragma once

UENUM(BlueprintType)
enum class EInputContext : uint8
{
	EIC_Default UMETA(DisplayName = "Default Mapping Context"),
	EIC_Combat UMETA(DisplayName = "Combat Mapping Context"),
	EIC_None UMETA(DisplayName = "No Mapping Context"),

	EA_MAX UMETA(DisplayName = "DefaultMAX")
};