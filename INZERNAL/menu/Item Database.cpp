#pragma once
#include <menu/menu.h>
#include <sdk/sdk.h>
#include <hooks/SendPacket.h>
#include <hooks/SendPacketRaw.h>
#include "items.h"
#include<PathFinder/Astar.hpp>

void menu::itemdata() {
    static ImGuiTextFilter draw;
    ImGui::Text("Item Count : %d", itemCount);
    ImGui::Text("Search:");
    ImGui::SameLine();
    draw.Draw("##searchbar", 200.f); // 340  < 400 
    ImGui::BeginChild("listbox child", AUTOSIZE(21.8)); //ImVec2(340, 400)
    for (int i = 0; i < iteminfo.size(); i++) {
        auto& items = iteminfo.at(i).name;
        if (draw.PassFilter(items.c_str())) {
            std::string items2 = items + "##" + std::to_string(i);
            if (ImGui::CollapsingHeader(items2.c_str())) {
                ImGui::Text("ItemID : %d", iteminfo.at(i).itemID);
                ImGui::SameLine();
                if (ImGui::Button("Copy ItemID")) {
                    ImGui::LogToClipboard();
                    ImGui::LogText("%d", iteminfo.at(i).itemID);
                    ImGui::LogFinish();
                }
                ImGui::Text("Rarity : %d", iteminfo.at(i).rarity);
                ImGui::Text("GrowTime : %d", iteminfo.at(i).growTime);
                ImGui::Text("BreakHits : %d", iteminfo.at(i).breakHits);
                ImGui::Text("Colission : %d", iteminfo.at(i).collisionType);
                if (ImGui::Button("Add Item To Inventory")) {
                    GameUpdatePacket p{};
                    p.type = 13;
                    p.int_data = iteminfo.at(i).itemID;
                    p.count2 = 1;
                    gt::send_self(&p);
                }
            }
        }
    }
}
