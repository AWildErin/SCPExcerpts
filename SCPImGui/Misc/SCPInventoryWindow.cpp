#include "Windows/Misc/SCPInventoryWindow.h"

#include "DevUIWindow.h"
#include "Components/SCPInventoryComponent.h"
#include "Items/SCPBaseItem.h"
#include "Items/SCPBaseContainerItem.h"

bool USCPInventoryWindow::Draw(UWorld* World)
{
	// Clear inventory if it becomes invalid
	if (!IsValid(SelectedInventory) || !IsValid(SelectedInventory->GetOwner()))
	{
		SelectedInventory = nullptr;
	}

	for (TObjectIterator<USCPInventoryComponent> It; It; ++It)
	{
		USCPInventoryComponent* Inventory = *It;

		if (Inventory->GetWorld() != World)
		{
			continue;
		}

		AActor* Owner = Inventory->GetOwner();
		if (!Owner)
		{
			continue;
		}

		ImGui::PushID((void*)Inventory);

		ImGui::Text("%s", TCHAR_TO_ANSI(*Owner->GetName()));
		ImGui::SameLine();

		if (ImGui::Button(TCHAR_TO_ANSI(TEXT("Select inventory"))))
		{
			SelectedInventory = Inventory;
		}

		ImGui::PopID();
	}


	if (ImGui::Button(TCHAR_TO_ANSI(TEXT("Delect inventory"))))
	{
		SelectedInventory = nullptr;
	}

	if (SelectedInventory)
	{
		ImGui::SameLine();
		ImGui::Text("Selected Inventory: %s", TCHAR_TO_ANSI(*SelectedInventory->GetOwner()->GetName()));
		DrawInventoryContents(SelectedInventory);
	}

	return true;
}

FString USCPInventoryWindow::GetWindowName()
{
	return TEXT("Inventory Debugger");
}

FString USCPInventoryWindow::GetWindowHelp()
{
	return TEXT("");
}

ImGuiWindowFlags USCPInventoryWindow::GetWindowFlags() const
{
	return ImGuiWindowFlags_None;
}

void USCPInventoryWindow::DrawInventoryContents(USCPInventoryComponent* Inventory)
{
	if (!Inventory)
	{
		return;
	}

	TArray<ASCPBaseItem*> Array = Inventory->GetInventoryArray();

	ImGui::Text("Inventory Contents:");

	if (ImGui::BeginTable("InventoryTable", 3))
	{
		for (int Row = 0; Row < Array.Num(); Row++)
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%d", Row);

			auto Item = Array[Row];
			ImGui::TableSetColumnIndex(1);
			if (Item)
			{
				ImGui::Text("%s", TCHAR_TO_ANSI(*Item->GetName()));
			}
			else
			{
				ImGui::Text("None");
			}

			ImGui::TableSetColumnIndex(2);
			if (Item)
			{
				if (ImGui::Button("Drop Item"))
				{
					Inventory->DropItem(Item);
				}
			}
		}
		ImGui::EndTable();
	}

	// Display what classes can be added
	ASCPBaseContainerItem* Container = Cast<ASCPBaseContainerItem>(Inventory->GetOwner());
	if (Container && Container->AllowedItemClassesList.Num() > 0)
	{
		ImGui::Text("Allowed item classes:");

		TArray<TSubclassOf<ASCPBaseItem>> AllowedItems = Container->AllowedItemClassesList;
		for (auto& ItemClass : AllowedItems)
		{
			ImGui::Text("%s", TCHAR_TO_ANSI(*ItemClass->GetName()));
		}
	}
}
