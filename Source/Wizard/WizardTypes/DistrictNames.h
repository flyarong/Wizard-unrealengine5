#pragma once

UENUM(BlueprintType)
enum class EDistrict : uint8
{
	ED_ShimmidgotBazaar UMETA(DisplayName = "Shimmidgot Bazaar"),
	ED_LowerEastPum UMETA(DisplayName = "Lower East Pum"), 
	ED_UpperSigguk UMETA(DisplayName = "Upper Sigguk"),
	ED_YiwoamCorner UMETA(DisplayName = "Yiwoam Corner"),
	ED_WatersidePumid UMETA(DisplayName = "Waterside Pumid"), 
	ED_DroskSquare UMETA(DisplayName = "Drosk Square"),
	ED_NorthChoffelt UMETA(DisplayName = "North Choffelt"),

	ED_MAX UMETA(DisplayName = "DefaultMAX")
};