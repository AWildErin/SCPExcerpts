#pragma once

#include "CoreMinimal.h"

#include "IPropertyTypeCustomization.h"

#include "Map/SCPRoomEnums.h"

/**
* @class FSCPZoneSelectCustomization
* Largely adapted from
*	https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Editor/DetailCustomizations/Private/LightingChannelsCustomization.h
*	https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Editor/DetailCustomizations/Private/LightingChannelsCustomization.cpp
*/
class FSCPZoneSelectCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// Begin IPropertyTypeCustomization Interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder,
								   IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	// End IPropertyTypeCustomization Interface

private:
	FText GetStructPropertyNameText() const;
	FText GetStructPropertyTooltipText() const;

	void CreateHeaderZoneCheckbox(const FString& LongName, const FString& ShortName, ESCPRoomZone Zone, TSharedRef<SHorizontalBox> ButtonGroup);

	void OnButtonCheckedStateChanged(ECheckBoxState NewState, ESCPRoomZone Zone) const;
	ECheckBoxState GetButtonCheckedState(ESCPRoomZone Zone) const;

	TSharedPtr<IPropertyHandle> StructHandle;
};
