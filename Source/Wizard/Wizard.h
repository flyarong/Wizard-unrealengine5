// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWizard, Log, All);

// Maps
#define DEFAULT_STORY_MAP FString("/Game/Wizard/Maps/Nildum")

// DataTables
#define CHARACTER_DATA_TABLE_PATH TEXT("/Script/Engine.DataTable'/Game/Wizard/Blueprints/Lobby/CharacterSelectorDataTable.CharacterSelectorDataTable'")
#define AGILITY_ITEM_DATA_TABLE_PATH TEXT("/Script/Engine.DataTable'/Game/Wizard/Blueprints/Stores/DataTables/AgilityItemDataTable.AgilityItemDataTable'")
#define COMBAT_ITEM_DATA_TABLE_PATH TEXT("/Script/Engine.DataTable'/Game/Wizard/Blueprints/Stores/DataTables/CombatItemDataTable.CombatItemDataTable'")
#define ENERGY_ITEM_DATA_TABLE_PATH TEXT("/Script/Engine.DataTable'/Game/Wizard/Blueprints/Stores/DataTables/EnergyItemDataTable.EnergyItemDataTable'")
#define HEALTH_ITEM_DATA_TABLE_PATH TEXT("/Script/Engine.DataTable'/Game/Wizard/Blueprints/Stores/DataTables/HealthItemDataTable.HealthItemDataTable'")
#define INTELLIGENCE_ITEM_DATA_TABLE_PATH TEXT("/Script/Engine.DataTable'/Game/Wizard/Blueprints/Stores/DataTables/IntelligenceItemDataTable.IntelligenceItemDataTable'")
#define POWER_ITEM_DATA_TABLE_PATH TEXT("/Script/Engine.DataTable'/Game/Wizard/Blueprints/Stores/DataTables/PowerItemDataTable.PowerItemDataTable'")
#define WISDOM_ITEM_DATA_TABLE_PATH TEXT("/Script/Engine.DataTable'/Game/Wizard/Blueprints/Stores/DataTables/WisdomItemDataTable.WisdomItemDataTable'")