#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <Vendor/imgui/imgui.h>
#include <Vendor/imgui/imgui_stdlib.h>
#include <Vendor/imgui/imgui_internal.h>
#include <LilEngie.h>
#include <Core/System/ISerializable.h>
#include <Core/Entity/ComponentFactory.h>
#include "../ImGuiStyle.h"
#include "IEditorWindow.h"
#include "LilTreeWindow.h"
#include "PropertiesWindow.h"

namespace LilEddie
{
	void PropertiesWindow::Init()
	{
		scnMgr = &game->sceneManager;
	}

	void PropertiesWindow::OnDraw()
	{
		Actor* sa = scnMgr->scene->GetActor(treeWindow->selected);

		if (sa)
		{
			//Name and uid
			ImGui::InputText("Name", &sa->name);
			ImGui::InputText("uid", &sa->uid);
			ImGui::Dummy(ImVec2(0, 20));

			//Add component button
			float width = ImGui::GetWindowWidth();
			if (ImGui::Button("New Component", ImVec2(width - 20, 25)))
				ImGui::OpenPopup("New Component");

			if (ImGui::BeginPopup("New Component"))
			{
				for (int i = 0; i < ComponentFactory::core->componentNames.size(); i++)
				{
					if (ImGui::Button(ComponentFactory::core->componentNames[i].c_str()))
					{
						//Add the component if not already existing
						std::string componentType = ComponentFactory::core->componentNames[i];
						if (!sa->ContainsComponent(componentType))
							IComponent* comp = ComponentFactory::core->CreateComponent(sa, componentType.c_str());

						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}

			//Component properties
			//Get json data
			json j;
			sa->Serialize(j);

			//Present each individual component
			for (int i = 0; i < sa->ComponentsCount(); i++)
			{
				std::string typeName = sa->GetComponent(i)->TypeName();
				current = sa->GetComponent(i);

				//Kind of a slow solution, iterating components then checking each json comp for match
				for (auto& component : j["components"])
				{
					if (component["type"] == typeName)
					{
						std::string type = component["type"].get<std::string>();

						//Split draw list to draw out of order foreground first
						ImDrawList* drawList = ImGui::GetWindowDrawList();
						drawList->ChannelsSplit(2);
						drawList->ChannelsSetCurrent(1);

						//Draw the properties dropdown in a group so that we can find the size
						ImGui::Dummy(ImVec2(0, 10));
						ImGui::BeginGroup();

						//Draw the header for the component
						ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;
						headerFlags |= ImGuiTreeNodeFlags_Framed;
						headerFlags |= ImGuiTreeNodeFlags_FramePadding;
						headerFlags |= ImGuiTreeNodeFlags_SpanAvailWidth;
						headerFlags |= ImGuiTreeNodeFlags_AllowItemOverlap;

						bool componentHeader = ImGui::TreeNodeEx((type + "##CollapseHeader").c_str(), headerFlags);

						//Draw the destroy component button
						ImGui::SameLine(ImGui::GetContentRegionAvail().x);
						ImGui::Dummy(ImVec2(10, 10));
						if (ImGui::CloseButton(ImGui::GetID(("x##" + type).c_str()), ImGui::GetItemRectMin()))
							sa->DestroyComponent(sa->GetComponent(i));

						//Draw actual component properties
						if (componentHeader)
						{
							ImGui::Indent();
							for (json::iterator it = component["properties"].begin(); it != component["properties"].end(); it++)
								DrawProperty(it.key(), it.value());
							ImGui::Unindent();
							ImGui::Dummy(ImVec2(0, 10));

							ImGui::TreePop();
						}
						ImGui::EndGroup();

						//Draw the background after we know the size
						drawList->ChannelsSetCurrent(0);

						ImColor color = Gradient(0) * .8;
						ImVec2 min = ImGui::GetItemRectMin();
						ImVec2 max = ImGui::GetItemRectMax();
						max.x = min.x + ImGui::GetContentRegionAvailWidth();
						drawList->AddRectFilled(min, max, color, 5);

						drawList->ChannelsMerge();
					}
				}
			}
		}
	}

	void PropertiesWindow::DrawProperty(std::string name, json value)
	{
		switch (value.type())
		{
			case json::value_t::null:
				ImGui::TextDisabled((name + " : NULL TYPE").c_str());
				break;
			case json::value_t::string:
			{
				std::string s = value.get<std::string>();
				if (ImGui::InputText(name.c_str(), &s) && current)
					current->SetProperty(name, s);
				break;
			}
			case json::value_t::boolean:
			{
				bool b = value.get<bool>();
				if (ImGui::Checkbox(name.c_str(), &b) && current)
					current->SetProperty(name, b);
				break;
			}
			case json::value_t::number_integer:
			{
				int i = value.get<int>();
				if (ImGui::DragInt(name.c_str(), &i) && current)
					current->SetProperty(name, i);
				break;
			}
			case json::value_t::number_unsigned:
			{
				int i = value.get<int>();
				if (ImGui::DragInt(name.c_str(), &i) && current)
					current->SetProperty(name, i);
				break;
			}
			case json::value_t::number_float:
			{
				float f = value.get<float>();
				if (ImGui::DragFloat(name.c_str(), &f) && current)
					current->SetProperty(name, f);
				break;
			}
			case json::value_t::array:
			{
				DrawArrayProperty(name, value);
				break;
			}
			case json::value_t::object:
			case json::value_t::discarded:
			default:
				ImGui::TextDisabled((name + " : NOT DISPLAYABLE").c_str());
				break;
		}
	}

	void PropertiesWindow::DrawArrayProperty(std::string& name, json arr)
	{
		//Only draw like this if under 4 elements (TODO temp)
		if (arr.size() > 4)
		{
			ImGui::TextDisabled((name + " : NOT DISPLAYABLE").c_str());
			return;
		}

		//Draw properties (TODO: add other types when im not lazy)
		switch (arr[0].type())
		{
			case json::value_t::string:
			{
				std::vector<std::string> vals = arr.get<std::vector<std::string>>();

				//Only show for resource id's
				if (vals.size() != 2 || resourceTypeNames.find(vals[1]) == resourceTypeNames.end())
				{
					ImGui::TextDisabled((name + " : NOT DISPLAYABLE").c_str());
					return;
				}

				//Make sure string size is large enough
				vals[0].resize(255);

				//Display property
				ImGui::Text(name.c_str());

				ImGui::Indent();
				bool mod = false;
				mod = ImGui::InputText(("##" + name).c_str(), &vals[0]);
				
				//Display type
				if (ImGui::BeginCombo(("Type##" + name).c_str(), vals[1].c_str()))
				{
					for (auto it = resourceTypeNames.begin(); it != resourceTypeNames.end(); it++)
					{
						bool isSelected = it->first == vals[1];
						if (ImGui::Selectable(it->first.c_str(), isSelected))
						{
							vals[1] = it->first;
							mod = true;
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Unindent();

				//Apply if modified
				if (mod && current)
				{
					//Set if it does
					ResourceId rid = ResourceId(vals[0], resourceTypeNames[vals[1]]);
					current->SetProperty(name, rid);
				}

				break;
			}
			case json::value_t::number_float:
			{
				//Only show for vec3/vec4
				if (arr.size() != 3 && arr.size() != 4)
				{
					ImGui::TextDisabled((name + " : NOT DISPLAYABLE").c_str());
					return;
				}

				std::vector<float> vals = arr.get<std::vector<float>>();
				bool mod = ImGui::DragScalarN(name.c_str(), ImGuiDataType_Float, &vals[0], vals.size(), .1f, 0, 0, "%.5f");

				//assume vec3/4, TODO: somehow figure out how to distinguish between vecs and arrays
				if (mod && current)
				{
					if (arr.size() == 3)
						current->SetProperty(name, vec3(vals[0], vals[1], vals[2]));
					else if (arr.size() == 4)
						current->SetProperty(name, vec4(vals[0], vals[1], vals[2], vals[3]));
				}

				break;
			}
			default:
				ImGui::TextDisabled((name + " : NOT DISPLAYABLE").c_str());
				break;
		}
	}
}
