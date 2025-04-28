#include "DetailCustomizations/SCPZoneSelectCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyHandle.h"

#include "Map/SCPRoomDataAsset.h"
#include "SCPEditorModule.h"

#define LOCTEXT_NAMESPACE "ZoneSelectCustomization"

TSharedRef<IPropertyTypeCustomization> FSCPZoneSelectCustomization::MakeInstance()
{
	return MakeShareable(new FSCPZoneSelectCustomization());
}

void FSCPZoneSelectCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	UE_LOG(LogSCPEditor, Warning, TEXT("Customize header called"));

	StructHandle = StructPropertyHandle;
	TSharedRef<SHorizontalBox> ButtonOptionsPanel = SNew(SHorizontalBox).Visibility(EVisibility::SelfHitTestInvisible);

	HeaderRow
		.NameContent()[SNew(STextBlock)
						   .Text(this, &FSCPZoneSelectCustomization::GetStructPropertyNameText)
						   .ToolTipText(this, &FSCPZoneSelectCustomization::GetStructPropertyTooltipText)
						   .Font(IDetailLayoutBuilder::GetDetailFont())
						   .Visibility(EVisibility::SelfHitTestInvisible)]
		.ValueContent()[ButtonOptionsPanel];

	CreateHeaderZoneCheckbox("Light Containment Zone", "LCZ", ESCPRoomZone::LightContainmentZone, ButtonOptionsPanel);
	CreateHeaderZoneCheckbox("Heavy Containment Zone", "HCZ", ESCPRoomZone::HeavyContainmentZone, ButtonOptionsPanel);
	CreateHeaderZoneCheckbox("Entrance Zone", "EZ", ESCPRoomZone::EntranceZone, ButtonOptionsPanel);
}

void FSCPZoneSelectCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
													IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	// Display channels as a normal foldout struct
	uint32 ChildCount;
	StructPropertyHandle->GetNumChildren(ChildCount);

	for (uint32 ChildIndex = 0; ChildIndex < ChildCount; ChildIndex++)
	{
		if (TSharedPtr<IPropertyHandle> ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex))
		{
			StructBuilder.AddProperty(ChildHandle.ToSharedRef());
		}
	}
}

FText FSCPZoneSelectCustomization::GetStructPropertyNameText() const
{
	if (StructHandle.IsValid())
	{
		return StructHandle->GetPropertyDisplayName();
	}

	return FText::GetEmpty();
}

FText FSCPZoneSelectCustomization::GetStructPropertyTooltipText() const
{
	if (StructHandle.IsValid())
	{
		return StructHandle->GetToolTipText();
	}

	return FText::GetEmpty();
}

void FSCPZoneSelectCustomization::CreateHeaderZoneCheckbox(const FString& LongName, const FString& ShortName, ESCPRoomZone Zone, TSharedRef<SHorizontalBox> ButtonGroup)
{
	// Our clang format really hates Slate...
	// clang-format off
	FString ToolTip = FString::Printf(TEXT("Can this zone spawn in the %s"), *LongName);

	ButtonGroup->AddSlot()
	.HAlign(HAlign_Left)
	.VAlign(VAlign_Center)
	.AutoWidth()
	.Padding(0, 0, 8, 0)
	[
		SNew(SBox)
		.WidthOverride(32)
		.HAlign(HAlign_Fill)
		[
			SNew(SCheckBox)
			.Style(&FAppStyle::Get().GetWidgetStyle<FCheckBoxStyle>("DetailsView.ChannelToggleButton"))
			.ToolTipText(FText::FromString(ToolTip))
			.OnCheckStateChanged(this, &FSCPZoneSelectCustomization::OnButtonCheckedStateChanged, Zone)
			.IsChecked(this, &FSCPZoneSelectCustomization::GetButtonCheckedState, Zone)
			.HAlign(HAlign_Center)
			.Padding(FMargin(0, 2))
			[
				SNew(STextBlock)
				.Font(FAppStyle::Get().GetFontStyle("SmallText"))
				.Visibility(EVisibility::HitTestInvisible)
				.Text(FText::FromString(ShortName))
				.ColorAndOpacity(FSlateColor::UseForeground())
			]
		]
	];

	// clang-format on
}

void FSCPZoneSelectCustomization::OnButtonCheckedStateChanged(ECheckBoxState NewState, ESCPRoomZone Zone) const
{
	if (!StructHandle.IsValid())
	{
		return;
	}

	UE_LOG(LogSCPEditor, Log, TEXT("Checkbox state was %d and zone was %d"), NewState, Zone);

	uint32 NumChildren = 0;
	StructHandle->GetNumChildren(NumChildren);

	int SelectedIndex = INDEX_NONE;

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ChildIndex++)
	{
		uint8 ChildValue = 0;
		StructHandle->GetChildHandle(ChildIndex)->GetValue(ChildValue);

		// Check to see if we're unchecking, and if the zone is equal to what we set
		if (NewState == ECheckBoxState::Unchecked && ChildValue == (uint8)Zone)
		{
			SelectedIndex = ChildIndex;
			break;
		}
		// If we're checking the box, find the next free index to set
		else if (NewState == ECheckBoxState::Checked && ChildValue == (uint8)ESCPRoomZone::None)
		{
			SelectedIndex = ChildIndex;
			break;
		}
	}

	if (SelectedIndex == INDEX_NONE)
	{
		UE_LOG(LogSCPEditor, Error, TEXT("Couldn't find index for SCPZoneSelect property. Checkbox state was %d and zone was %d"), NewState, Zone);
		return;
	}

	UE_LOG(LogSCPEditor, Log, TEXT("Setting index %d"), SelectedIndex);
	TSharedPtr<IPropertyHandle> PropHandle = StructHandle->GetChildHandle(SelectedIndex);

	// Reset the zone
	if (NewState == ECheckBoxState::Unchecked)
	{
		StructHandle->GetChildHandle(SelectedIndex)->SetValue((uint8)ESCPRoomZone::None, EPropertyValueSetFlags::NotTransactable);
	}
	else if (NewState == ECheckBoxState::Checked)
	{
		StructHandle->GetChildHandle(SelectedIndex)->SetValue((uint8)Zone, EPropertyValueSetFlags::NotTransactable);
	}

	StructHandle->GetChildHandle(SelectedIndex)->NotifyFinishedChangingProperties();
	StructHandle->NotifyFinishedChangingProperties();
}

ECheckBoxState FSCPZoneSelectCustomization::GetButtonCheckedState(ESCPRoomZone Zone) const
{
	if (!StructHandle.IsValid())
	{
		return ECheckBoxState::Undetermined;
	}

	uint32 NumChildren = 0;
	StructHandle->GetNumChildren(NumChildren);

	int SelectedIndex = INDEX_NONE;

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ChildIndex++)
	{
		uint8 ChildValue = 0;
		StructHandle->GetChildHandle(ChildIndex)->GetValue(ChildValue);

		if (ChildValue == (uint8)Zone)
		{
			return ECheckBoxState::Checked;
		}
	}

	return ECheckBoxState::Unchecked;
}

#undef LOCTEXT_NAMESPACE
