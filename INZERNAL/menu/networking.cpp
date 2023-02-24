#pragma once
#include <menu/menu.h>
#include <sdk/sdk.h>
//#include <proton\VariantDB.h>
#include<hooks/ProcessTankUpdatePacket.h>

void menu::networking_tab() {
    if (ImGui::BeginTabBar("##net", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Net 1")) {
            ImGui::BeginChild("Net 1");
            auto client = sdk::GetClient();
            bool client_div = ImGui::BeginChild("ENetClient", AUTOSIZEC(4.9), true, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);
            if (client_div && client) {
                ImGui::BeginMenuBar();
                ImGui::Text("ENetClient");
                ImGui::EndMenuBar();
                ImGui::Text("Address: %s", client->address.c_str());
                ImGui::Text("Port: %u", client->port);
                ImGui::Text("User: %u", client->user);
                ImGui::Text("Token: %u", client->token);
                ImGui::Text("Msg Type: %u", client->net_msg_type);
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###loginspoof", AUTOSIZEC(3.1), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Login Spoof");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Spoof Login   ", &opt::spoof_login);
                ImGui::SameLine();
                ImGui::Checkbox("Random Flag   ", &opt::cheat::random_flag);
                ImGui::SameLine();
                ImGui::Checkbox("Random Name   ", &opt::spoof_name);
                ImGui::Checkbox("Spoof Version   ", &opt::verspoof);
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.08f);
                ImGui::InputText("##ver", opt::gt_version, CHAR_MAX);
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###tileinfo", AUTOSIZEC(5.3), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Tile Info");
                ImGui::EndMenuBar();
                if (sdk::GetGameLogic()->GetLocalPlayer()) {
                    static int x = 0, y = 0;
                    typedef pair<int, int> Pair;
                    Pair dest((int)opt::cheat::TouchPos.x, (int)opt::cheat::TouchPos.y);
                    auto world = sdk::GetGameLogic()->GetWorld();
                    auto tilemap = sdk::GetGameLogic()->GetTileMap();
                    static Tile* tile = tilemap->GetTileSafe(dest.first, dest.second);

                    auto objmap = sdk::GetGameLogic()->GetObjectMap();

                    ImGui::Checkbox("Enable", &opt::cheat::tileinfo);
                    if (!opt::cheat::tileinfo) {
                        ImGui::Spacing();
                        ImGui::TextColored(ImVec4(0.50f, 0.50f, 0.50f, 1.00f), "Pos: NULL");
                        ImGui::TextColored(ImVec4(0.50f, 0.50f, 0.50f, 1.00f), "Foreground: NULL");
                        ImGui::TextColored(ImVec4(0.50f, 0.50f, 0.50f, 1.00f), "Background: NULL");
                        ImGui::TextColored(ImVec4(0.50f, 0.50f, 0.50f, 1.00f), "Flags: NULL");
                    }
                    if (opt::cheat::tileinfo) {
                        int xx = dest.first;
                        int yx = dest.second;
                        ImGui::PushItemWidth(150.f);
                        if (xx) {
                            if (xx > 99)
                                xx = 99;
                            else if (xx < 0)
                                xx = 0;
                            tile = tilemap->GetTileSafe(xx, yx);
                        }

                        if (yx) {
                            if (yx > 59)
                                yx = 59;
                            else if (dest.second / 32 < 0)
                                yx = 0;
                            tile = tilemap->GetTileSafe(xx, yx);
                        }

                        ImGui::PopItemWidth();
                        ImGui::Spacing();
                        if (tile) {
                            ImGui::Text("Pos: %d, %d", tile->position_x, tile->position_y);
                            ImGui::Text("Foreground: %d", tile->foreground);
                            ImGui::Text("Background: %d", tile->background);
                            ImGui::Text("Flags: %d", tile->flags);
                        }
                    }
                }
                else
                    ImGui::TextColored(ImVec4(231.0f / 255.0f, 76.0f / 255.0f, 60.0f / 255.0f, 1.0f), "Enter A World To Access Tile Info");
                ImGui::EndChild();
                if (ImGui::BeginChild("###autocaptcha", AUTOSIZEC(2.2), true, ImGuiWindowFlags_MenuBar)) {
                    ImGui::BeginMenuBar();
                    ImGui::Text("Auto Captcha");
                    ImGui::EndMenuBar();
                    ImGui::Checkbox("Enable", &opt::cheat::solve_captcha);
                    ImGui::EndChild();
                }
                if (ImGui::BeginChild("###moddetector", AUTOSIZEC(2.2), true, ImGuiWindowFlags_MenuBar)) {
                    ImGui::BeginMenuBar();
                    ImGui::Text("Mod Detector");
                    ImGui::EndMenuBar();
                    ImGui::Checkbox("Leave When Mod Enterns", &opt::cheat::modleave);
                    ImGui::EndChild();
                }

            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Net 2")) {
            ImGui::BeginChild("Net 2");
            if (ImGui::BeginChild("###console", AUTOSIZEC(2.2), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("idfk");
                ImGui::EndMenuBar();
                if (ImGui::Button("Ban Current Account"))
                    ImGui::OpenPopup("##popup");
                ImGui::SameLine();
                static int selectedd = -1;
                const char* namess[] = { "Yes", "No" };
                if (ImGui::BeginPopup("##popup"))
                {
                    ImGui::Text("Are You Sure?");
                    ImGui::Separator();
                    for (int i = 0; i < IM_ARRAYSIZE(namess); i++) {
                        if (ImGui::Selectable(namess[i])) {
                            selectedd = i;
                        }
                        if (selectedd == 0) {
                            auto la = sdk::GetGameLogic()->GetLocalPlayer();
                            for (int i = 0; i < 1000; i++) {
                                GameUpdatePacket Punch{ 0 };
                                auto pos = la->GetPos();
                                Punch.type = PACKET_STATE;
                                Punch.int_data = 0;
                                Punch.int_x = 10000;
                                Punch.int_y = 10000;
                                Punch.flags = 8390688;
                                Punch.pos_x = pos.x;
                                Punch.pos_y = pos.y;
                                Punch.vec_x = pos.x;
                                Punch.vec_y = pos.y;
                                Punch.vec2_x = 0;
                                Punch.vec2_y = 0;
                                gt::send(&Punch);
                            }
                            selectedd = -1;
                        }
                        if (selectedd == 1) {
                            selectedd = -1;
                        }
                    }
                    ImGui::EndPopup();
                }
                ImGui::EndChild();
            }
        }
        ImGui::EndChild();
        ImGui::EndTabItem();


    }
}
