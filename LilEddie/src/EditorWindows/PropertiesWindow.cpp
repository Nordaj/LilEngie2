#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
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

		ImGui::Begin("Properties");

		if (sa)
		{
			//Get json data
			json j;
			sa->Serialize(j);

			//Display each components data
			for (auto& component : j["components"])
			{
				ImGui::Text(component["type"].get<std::string>().c_str());
				ImGui::Indent();
				for (json::iterator it = component["properties"].begin(); it != component["properties"].end(); it++)
				{
					DrawProperty(it.key(), it.value());
				}
				ImGui::Unindent();
				ImGui::Separator();
			}
		}

		ImGui::End();
	}

	//TODO: does not yet persist data
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
				ImGui::InputText(name.c_str(), &s[0], s.size());
				break;
			}
			case json::value_t::boolean:
			{
				bool b = value.get<bool>();
				ImGui::Checkbox(name.c_str(), &b);
				break;
			}
			case json::value_t::number_integer:
			{
				int i = value.get<int>();
				ImGui::DragInt(name.c_str(), &i);
				break;
			}
			case json::value_t::number_unsigned:
			{
				int i = value.get<int>();
				ImGui::DragInt(name.c_str(), &i);
				break;
			}
			case json::value_t::number_float:
			{
				float f = value.get<float>();
				ImGui::DragFloat(name.c_str(), &f);
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
				ImGui::Text(name.c_str());

				ImGui::Indent();
				for (std::string& s : vals)
					ImGui::InputText("", &s[0], s.size());
				ImGui::Unindent();

				break;
			}
			case json::value_t::number_float:
			{
				std::vector<float> vals = arr.get<std::vector<float>>();
				ImGui::DragScalarN(name.c_str(), ImGuiDataType_Float, &vals[0], vals.size(), .1f, 0, 0, "%.2f");
				break;
			}
			default:
				ImGui::TextDisabled((name + " : NOT DISPLAYABLE").c_str());
				break;
		}
	}
}
