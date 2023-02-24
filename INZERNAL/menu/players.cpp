#pragma once
#include <menu/menu.h>
#include <sdk/sdk.h>
#include <hooks/SendPacket.h>
#include <hooks/SendPacketRaw.h>

void menu::player_tab() {
    if (ImGui::BeginChild("###playertools", AUTOSIZEC(2.2), true, ImGuiWindowFlags_MenuBar)) {
        ImGui::BeginMenuBar();
        ImGui::Text("Player Tools");
        ImGui::EndMenuBar();
        if (ImGui::Button("Ban All")) {
            for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
                std::string name = pair.second->name;
                std::string name2 = name.substr(2);
                utils::replace(name2, "``", "");
                std::string packet = "action|input\n|text|/ban " + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Pull All")) {
            for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
                std::string name = pair.second->name;
                std::string name2 = name.substr(2);
                utils::replace(name2, "``", "");
                std::string packet = "action|input\n|text|/pull " + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Kick All")) {
            for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
                std::string name = pair.second->name;
                std::string name2 = name.substr(2);
                utils::replace(name2, "``", "");
                std::string packet = "action|input\n|text|/kick " + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Remove All")) {
            auto local = sdk::GetGameLogic()->GetLocalPlayer();
            for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
                if (pair.first != local->netid) {
                    variantlist_t varlst2{ "OnRemove" };
                    varlst2[1] = "netID|" + std::to_string(pair.first);
                    gt::send_varlist_self(varlst2);
                }
            }
        }
        ImGui::SameLine();
        ImGui::Checkbox("PlayerEsp", &opt::cheat::playeresp);
        ImGui::EndChild();
    }
    if (ImGui::BeginChild("###playerlist", AUTOSIZEC(NULL), true, ImGuiWindowFlags_MenuBar)) {
        ImGui::BeginMenuBar();
        ImGui::Text("Player List");
        ImGui::EndMenuBar();
        static bool selected1 = false;
        static int selected = -1;
        const char* names[] = { "Kick", "Ban", "Pull", "Oblidirate", "Close" };
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            std::string name = pair.second->name;
            std::string name2 = name.substr(2);
            utils::replace(name2, "``", "");
            ImGui::Selectable(std::string(name).c_str(), selected1);
            if (selected == 0) {
                std::string packet = "action|input\n|text|/kick " + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
                selected = -1;
                selected1 = false;
            }
            if (selected == 1) {
                std::string packet = "action|input\n|text|/ban " + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
                selected = -1;
                selected1 = false;
            }
            if (selected == 2) {
                std::string packet = "action|input\n|text|/pull " + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
                selected = -1;
                selected1 = false;
            }
            if (selected == 3) {
                std::string packet = "action|input\n|text|/ignore /" + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
                variantlist_t varlst2{ "OnRemove" };
                varlst2[1] = "netID|" + std::to_string(pair.first);
                gt::send_varlist_self(varlst2);
                variantlist_t varlst{ "OnParticleEffect" };
                varlst[1] = 97;
                varlst[2] = vector2_t{ pair.second->pos.x + 10, pair.second->pos.y + 15 };
                varlst[3] = 0;
                varlst[4] = 0;
                gt::send_varlist_self(varlst);
                selected = -1;
                selected1 = false;
            }
            if (selected == 4) {
                selected = -1;
                selected1 = false;
            }
            if (selected1) {
                ImGui::OpenPopup("##commandpopup");
                if (ImGui::BeginPopup("##commandpopup"))
                {
                    ImGui::Text("What Would You Like To Do?");
                    ImGui::Separator();
                    for (int i = 0; i < IM_ARRAYSIZE(names); i++) {
                        if (ImGui::Selectable(names[i])) {
                            selected = i;
                        }
                    }
                    ImGui::EndPopup();

                }
            }
            
        }
        ImGui::EndChild();
    }
}