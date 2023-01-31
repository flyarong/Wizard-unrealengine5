#pragma once

UENUM(BlueprintType)
enum class EStore : uint8
{
	ES_General UMETA(DisplayName = "General Store"),
	ES_Forge UMETA(DisplayName = "Forge"),

	EA_MAX UMETA(DisplayName = "DefaultMAX")
};