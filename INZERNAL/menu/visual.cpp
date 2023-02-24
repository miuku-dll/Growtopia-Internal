#pragma once
#include <menu/menu.h>
#include <sdk/Consts.h>
#include <hooks/SendPacket.h>
#include<iostream>
#include<menu/etc_elements.h>
void menu::visual_tab() {
    static types::time times = std::chrono::system_clock::now();
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (ImGui::BeginTabBar("VSLtabs", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Visual 1")) {
            ImGui::BeginChild("Visual 1");
            if (ImGui::BeginChild("###deathanims", AUTOSIZEC(2.2), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Animations");
                ImGui::EndMenuBar();
                static  int stuff = 0;
                static const char* others[]{ "Select Spawn Anim", "Leaf Spawn", "Flame Spawn", "Celestial Spawn", "Default Spawn" };
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.30f);
                ImGui::Combo("##things", &stuff, others, IM_ARRAYSIZE(others));
                if (stuff == 1) {
                    gt::send(2, "action|setRespawn\nanimRespawn|1");
                    stuff = 0;
                }
                if (stuff == 2) {
                    gt::send(2, "action|setRespawn\nanimRespawn|2");
                    stuff = 0;
                }
                if (stuff == 3) {
                    gt::send(2, "action|setRespawn\nanimRespawn|3");
                    stuff = 0;
                }
                if (stuff == 4) {
                    gt::send(2, "action|setRespawn\nanimRespawn|0");
                    stuff = 0;
                }
                ImGui::SameLine();
                static  int stuffs = 0;
                static const char* otherss[]{ "Select Death Anim", "No Anim", "Void Flame", "Default Death" };
                ImGui::Combo("##thingss", &stuffs, otherss, IM_ARRAYSIZE(otherss));
                if (stuffs == 1) {
                    gt::send(2, "action|setDeath\nanimDeath|1");
                    stuffs = 0;
                }
                if (stuffs == 2) {
                    gt::send(2, "action|setDeath\nanimDeath|3");
                    stuffs = 0;
                }
                if (stuffs == 3) {
                    gt::send(2, "action|setDeath\nanimDeath|0");
                    stuffs = 0;
                }
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###name", AUTOSIZEC(3.1), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Visual Name");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Enable   ", &opt::visual::name);
                ImGui::SameLine();
                if (ImGui::Button("Apply Name") && opt::visual::name) {
                    variantlist_t varlist{ "OnNameChanged" };
                    varlist[1] = sdk::GetGameLogic()->GetLocalPlayer()->name = opt::visual::nametxt;
                    gt::send_varlist_self(varlist, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                }
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
                ImGui::InputText("##visualname", &opt::visual::nametxt);
                static  int stuffs = 0;
                static const char* otherss[]{ "Select A Title", "Max Level", "G4G", "Master", "Legend", "Doctor", "No Title" };
                ImGui::Checkbox("Titles   ", &opt::visual::title);
                ImGui::SameLine();
                ImGui::Combo("##thingss", &stuffs, otherss, IM_ARRAYSIZE(otherss));
                if (stuffs == 1) {
                    opt::visual::titletxt = "|showGuild|maxLevel";
                    std::string packet125level = local->country + "|showGuild|maxLevel";
                    variantlist_t packet123{ "OnCountryState" };
                    packet123[1] = packet125level.c_str();
                    if (local) {
                        gt::send_varlist_self(packet123, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                    }
                    stuffs = 0;
                }
                if (stuffs == 2) {
                    opt::visual::titletxt = "|showGuild|donor";
                    std::string packetg4g = local->country + "|showGuild|donor";
                    variantlist_t packet123{ "OnCountryState" };
                    packet123[1] = packetg4g.c_str();
                    if (local) {
                        gt::send_varlist_self(packet123, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                    }
                    stuffs = 0;
                }
                if (stuffs == 3) {
                    opt::visual::titletxt = "|showGuild|master";
                    std::string packetmaster = local->country + "|showGuild|master";
                    variantlist_t packet123{ "OnCountryState" };
                    packet123[1] = packetmaster.c_str();
                    if (local) {
                        gt::send_varlist_self(packet123, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                    }
                    stuffs = 0;
                }
                if (stuffs == 4) {
                    variantlist_t va{ "OnNameChanged" };
                    va[1] = opt::visual::nametxt + " of Legend``";
                    if (local) {
                        gt::send_varlist_self(va, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                    }
                    opt::visual::nametxt = opt::visual::nametxt + " of Legend``";
                    stuffs = 0;
                }
                if (stuffs == 5) {
                    variantlist_t va{ "OnNameChanged" };
                    va[1] = "`4Dr. " + opt::visual::nametxt;
                    if (local) {
                        gt::send_varlist_self(va, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                    }
                    opt::visual::nametxt = "`4Dr. " + opt::visual::nametxt;
                    stuffs = 0;
                }
                if (stuffs == 6) {
                    opt::visual::titletxt = "default";
                    opt::visual::title = false;
                    opt::visual::legendary = false;
                    opt::visual::dr = false;
                    std::string packetmaster = local->country + "|showGuild|noTitle";
                    variantlist_t packet123{ "OnCountryState" };
                    packet123[1] = packetmaster.c_str();
                    if (local) {
                        gt::send_varlist_self(packet123, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                    }
                    stuffs = 0;
                }
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###misc", AUTOSIZEC(6.9), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Visual Misc");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Guild Flag   ", &opt::visual::guildflag);
                ImGui::SameLine();
                if (ImGui::Button("Apply GF") && opt::visual::guildflag) {
                    variantlist_t va{ "OnGuildDataChanged" };
                    va[1] = 1;
                    va[2] = 2;
                    va[3] = opt::visual::setflagtoitemid;
                    va[4] = 3;
                    gt::send_varlist_self(va, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                }
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.17f);
                ImGui::InputInt("##1000", &opt::visual::setflagtoitemid);
                ImGui::SameLine();
                ImGui::Text("       ");
                ImGui::SameLine();
                if (ImGui::Button("VisualSaving"))
                    ImGui::OpenPopup("##popup");
                ImGui::SameLine();
                static int selectedd = -1;
                const char* namess[] = { "Save", "Load" };
                if (ImGui::BeginPopup("##popup"))
                {
                    ImGui::Text("xcvxc");
                    ImGui::Separator();
                    for (int i = 0; i < IM_ARRAYSIZE(namess); i++) {
                        if (ImGui::Selectable(namess[i])) {
                            selectedd = i;
                        }
                        if (selectedd == 0) {
                            gt::WriteConfig("JustRaped12KidsInTheWoods.ini");
                            variantlist_t varlst2{ "OnTextOverlay" };
                            varlst2[1] = "`5Saved Visuals To Config";
                            gt::send_varlist_self(varlst2);
                            selectedd = -1;
                            if (local) {
                                variantlist_t varlst{ "OnParticleEffect" };
                                varlst[1] = 48;
                                varlst[2] = vector2_t{ local->pos.x + 10, local->pos.y + 15 };
                                varlst[3] = 0;
                                varlst[4] = 0;
                                gt::send_varlist_self(varlst);
                            }
                        }
                        if (selectedd == 1) {
                            variantlist_t varlst2{ "OnTextOverlay" };
                            varlst2[1] = "`^Loaded Visuals From Config";
                            gt::send_varlist_self(varlst2);
                            gt::ReadConfig("JustRaped12KidsInTheWoods.ini");
                            if (local) {
                                variantlist_t varlst{ "OnParticleEffect" };
                                varlst[1] = 90;
                                varlst[2] = vector2_t{ local->pos.x + 10, local->pos.y + 15 };
                                varlst[3] = 0;
                                varlst[4] = 0;
                                gt::send_varlist_self(varlst);
                                if (opt::visual::clothes) {
                                    variantlist_t vlist{ "OnSetClothing" };
                                    vlist[1] = vector3_t{ (float)opt::visual::vitemidX1, (float)opt::visual::vitemidY1, (float)opt::visual::vitemidZ1 };
                                    vlist[2] = vector3_t{ (float)opt::visual::vitemidX2, (float)opt::visual::vitemidY2, (float)opt::visual::vitemidZ2 };
                                    vlist[3] = vector3_t{ (float)opt::visual::vitemidX3, (float)opt::visual::vitemidY3, (float)opt::visual::vitemidZ3 };
                                    vlist[4] = opt::visual::skincolor;
                                    vlist[5] = vector3_t{ (float)opt::visual::vitemidX5, 1, 0 };
                                    gt::send_varlist_self(vlist, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                                }
                                if (opt::visual::name) {
                                    variantlist_t varlist{ "OnNameChanged" };
                                    varlist[1] = opt::visual::nametxt;
                                    gt::send_varlist_self(varlist, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                                }
                                if (opt::visual::weather) {
                                    variantlist_t varlist{ "OnSetCurrentWeather" };
                                    varlist[1] = opt::visual::weatherint;
                                    gt::send_varlist_self(varlist);
                                }
                                if (opt::visual::guildflag) {
                                    variantlist_t va{ "OnGuildDataChanged" };
                                    va[1] = 1;
                                    va[2] = 2;
                                    va[3] = opt::visual::setflagtoitemid;
                                    va[4] = 3;
                                    gt::send_varlist_self(va, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                                }
                                if (opt::visual::role_enable) {
                                    variantlist_t va{ "OnSetRoleSkinsAndIcons" };
                                    va[1] = opt::visual::role;
                                    va[2] = opt::visual::role;
                                    gt::send_varlist_self(va, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                                }
                                if (opt::visual::title) {
                                    std::string packet125level = local->country + opt::visual::titletxt;
                                    variantlist_t packet123{ "OnCountryState" };
                                    packet123[1] = packet125level.c_str();
                                    gt::send_varlist_self(packet123, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
                                }
                                if (opt::visual::legendary) {
                                    variantlist_t va{ "OnNameChanged" };
                                    va[1] = "``" + local->name + " of Legend``";
                                    gt::send_varlist_self(va, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);

                                }
                                if (opt::visual::dr) {
                                    std::string pkt;
                                    pkt = "az|showGuild|doctor";
                                    variantlist_t va{ "OnNameChanged" };
                                    va[1] = "`4Dr. " + local->name;
                                    gt::send_varlist_self(va, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);

                                }
                            }
                            selectedd = -1;
                        }
                    }

                    ImGui::EndPopup();
                }
                ImGui::SameLine();
                ImGui::Checkbox("Load On Start", &opt::visual::loadonstart);
                static char flag[100] = "mp";
                if (ImGui::Button("Apply Flag")) {
                    variantlist_t liste{ "OnReconnect" };
                    gt::send_varlist_self(liste);
                    opt::cheat::random_flag = false;
                }
                ImGui::SameLine();
                ImGui::Text("Flag:");
                ImGui::SameLine();
                ImGui::InputText("##flag", flag, CHAR_MAX);
                opt::flag = flag;
                ImGui::Checkbox("Weather   ", &opt::visual::weather);
                ImGui::SameLine();
                if (ImGui::Button("Apply Weather") && opt::visual::weather) {
                    variantlist_t varlist{ "OnSetCurrentWeather" };
                    varlist[1] = opt::visual::weatherint;
                    gt::send_varlist_self(varlist);
                }
                ImGui::SameLine();
                ImGui::InputInt("##visualweather", &opt::visual::weatherint);
                ImGui::Checkbox("Particle   ", &opt::visual::particle);
                ImGui::SameLine();
                if (ImGui::Button("Send Particle") && opt::visual::particle) {
                    variantlist_t varlst{ "OnParticleEffect" };
                    varlst[1] = opt::visual::particleint;
                    varlst[2] = vector2_t{ local->pos.x + 10, local->pos.y + 15 };
                    varlst[3] = 0;
                    varlst[4] = 0;
                    gt::send_varlist_self(varlst);
                }
                ImGui::SameLine();
                ImGui::InputInt("##particleintt", &opt::visual::particleint);
                ImGui::Checkbox("Spam Particle", &opt::visual::spamparticle);
                ImGui::Checkbox("See fruits   ", &opt::cheat::see_fruits);
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);
                ImGui::SliderInt("##treesize", &opt::visual::treesize, 1, 200);
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###clothes", AUTOSIZEC(8.9), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Visual Clothes");
                ImGui::EndMenuBar();
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.17f);
                ImGui::Checkbox("Enable", &opt::visual::clothes);
                ImGui::Text("Hair");
                ImGui::SameLine();
                ImGui::InputInt("##1", &opt::visual::vitemidY3);
                ImGui::SameLine();
                ImGui::Text("Pants");
                ImGui::SameLine();
                ImGui::InputInt("##4", &opt::visual::vitemidZ1);
                ImGui::Text("Shirt");
                ImGui::SameLine();
                ImGui::InputInt("##2", &opt::visual::vitemidY1);
                ImGui::SameLine();
                ImGui::Text("Wing");
                ImGui::SameLine();
                ImGui::InputInt("##5", &opt::visual::vitemidX3);
                ImGui::Text("Shoes");
                ImGui::SameLine();
                ImGui::InputInt("##3", &opt::visual::vitemidX2);
                ImGui::SameLine();
                ImGui::Text("Hand");
                ImGui::SameLine();
                ImGui::InputInt("##6", &opt::visual::vitemidZ2);
                ImGui::Text("Neck");
                ImGui::SameLine();
                ImGui::InputInt("##7", &opt::visual::vitemidZ3);
                ImGui::SameLine();
                ImGui::Text("Face");
                ImGui::SameLine();
                ImGui::InputInt("##8", &opt::visual::vitemidY2);
                ImGui::Text("Ances");
                ImGui::SameLine();
                ImGui::InputInt("##9", &opt::visual::vitemidX5);
                ImGui::SameLine();
                ImGui::Text("Hat");
                ImGui::SameLine();
                ImGui::InputInt("##10", &opt::visual::vitemidX1);
                ImGui::Text("Skin Color");
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
                ImGui::InputInt("##11", &opt::visual::skincolor);
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
                if (ImGui::Button("Apply")) {
                    opt::visual::clothes = true;
                    variantlist_t vlist{ "OnSetClothing" };
                    vlist[1] = vector3_t{ (float)opt::visual::vitemidX1, (float)opt::visual::vitemidY1, (float)opt::visual::vitemidZ1 };
                    vlist[2] = vector3_t{ (float)opt::visual::vitemidX2, (float)opt::visual::vitemidY2, (float)opt::visual::vitemidZ2 };
                    vlist[3] = vector3_t{ (float)opt::visual::vitemidX3, (float)opt::visual::vitemidY3, (float)opt::visual::vitemidZ3 };
                    vlist[4] = opt::visual::skincolor;
                    vlist[5] = vector3_t{ (float)opt::visual::vitemidX5, 1, 0 };
                    gt::send_varlist_self(vlist, sdk::GetGameLogic()->GetLocalPlayer()->netid);
                }
                ImGui::SameLine();
                if (ImGui::Button("Reset")) {
                    opt::visual::vitemidY3 = 0;
                    opt::visual::vitemidZ1 = 0;
                    opt::visual::vitemidY1 = 0;
                    opt::visual::vitemidX1 = 0;
                    opt::visual::vitemidX3 = 0;
                    opt::visual::vitemidX2 = 0;
                    opt::visual::vitemidZ2 = 0;
                    opt::visual::vitemidZ3 = 0;
                    opt::visual::vitemidY2 = 0;
                    opt::visual::vitemidX5 = 0;
                    opt::visual::skincolor = 2190853119;
                }
                ImGui::EndChild();
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Visual 2")) {
            ImGui::BeginChild("Visual 2");
            if (ImGui::BeginChild("###role", AUTOSIZEC(2.2), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Visual Roles");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Enable   ", &opt::visual::role_enable);
                ImGui::SameLine();
                static  int role = 0;
                static const char* roles[]{ "Select A Role", "Builder", "Surgeon", "Fisher", "Cook", "Star Captain", "Farmer" };
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.30f);
                if (ImGui::Combo("##roles", &role, roles, IM_ARRAYSIZE(roles)) && opt::visual::role_enable) {
                    if (role == 1) {
                        opt::visual::role = 1;
                        variantlist_t vlist{ "OnSetRoleSkinsAndIcons" };
                        vlist[1] = 1;
                        vlist[2] = 1;
                        gt::send_varlist_self(vlist, sdk::GetGameLogic()->GetLocalPlayer()->netid);
                        role = 0;
                    }
                    if (role == 2) {
                        opt::visual::role = 2;
                        variantlist_t vlist{ "OnSetRoleSkinsAndIcons" };
                        vlist[1] = 2;
                        vlist[2] = 2;
                        gt::send_varlist_self(vlist, sdk::GetGameLogic()->GetLocalPlayer()->netid);
                        role = 0;
                    }
                    if (role == 3) {
                        opt::visual::role = 3;
                        variantlist_t vlist{ "OnSetRoleSkinsAndIcons" };
                        vlist[1] = 3;
                        vlist[2] = 3;
                        gt::send_varlist_self(vlist, sdk::GetGameLogic()->GetLocalPlayer()->netid);
                        role = 0;
                    }
                    if (role == 4) {
                        opt::visual::role = 4;
                        variantlist_t vlist{ "OnSetRoleSkinsAndIcons" };
                        vlist[1] = 4;
                        vlist[2] = 4;
                        gt::send_varlist_self(vlist, sdk::GetGameLogic()->GetLocalPlayer()->netid);
                        role = 0;
                    }
                    if (role == 5) {
                        opt::visual::role = 5;
                        variantlist_t vlist{ "OnSetRoleSkinsAndIcons" };
                        vlist[1] = 5;
                        vlist[2] = 5;
                        gt::send_varlist_self(vlist, sdk::GetGameLogic()->GetLocalPlayer()->netid);
                        role = 0;
                    }
                    if (role == 6) {
                        opt::visual::role = 6;
                        variantlist_t vlist{ "OnSetRoleSkinsAndIcons" };
                        vlist[1] = 0;
                        vlist[2] = 0;
                        gt::send_varlist_self(vlist, sdk::GetGameLogic()->GetLocalPlayer()->netid);
                        role = 0;
                    }
                }
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###textcolor", AUTOSIZEC(2.2), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Text Color");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Enable   ", &opt::visual::textcolorenable);
                ImGui::SameLine();
                static  int selected = 0;
                static const char* colors[]{ "Select A Color", "White", "Light Cyan", "Bright Cyan", "Green", "Pale Blue", "Red", "Wave Orange", "Light Grey", "Orange", "Yellow", "Black", "Bright Red",
                    "Pink Bubblegum", "Pale Yellow", "Very Pale Pink", "Dreamsicle", "Pink", "Dark Blue", "Medium Blue", "Pale Green", "Medium Green", "Dark Grey", "Medium Grey", "Vibrant Cyan" };
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.30f);
                if (ImGui::Combo("##roles", &selected, colors, IM_ARRAYSIZE(colors)) && opt::visual::textcolorenable) {
                    if (selected == 1) {
                        opt::visual::textcolor = "`w";
                    }
                    if (selected == 2) {
                        opt::visual::textcolor = "`1";
                    }
                    if (selected == 3) {
                        opt::visual::textcolor = "`!";
                    }
                    if (selected == 4) {
                        opt::visual::textcolor = "`2";
                    }
                    if (selected == 5) {
                        opt::visual::textcolor = "`3";
                    }
                    if (selected == 6) {
                        opt::visual::textcolor = "`4";
                    }
                    if (selected == 7) {
                        opt::visual::textcolor = "`6";
                    }
                    if (selected == 8) {
                        opt::visual::textcolor = "`7";
                    }
                    if (selected == 9) {
                        opt::visual::textcolor = "`8";
                    }
                    if (selected == 10) {
                        opt::visual::textcolor = "`9";
                    }
                    if (selected == 11) {
                        opt::visual::textcolor = "`b";
                    }
                    if (selected == 12) {
                        opt::visual::textcolor = "`@";
                    }
                    if (selected == 13) {
                        opt::visual::textcolor = "`#";
                    }
                    if (selected == 14) {
                        opt::visual::textcolor = "`$";
                    }
                    if (selected == 15) {
                        opt::visual::textcolor = "`&";
                    }
                    if (selected == 16) {
                        opt::visual::textcolor = "`o";
                    }
                    if (selected == 17) {
                        opt::visual::textcolor = "`p";
                    }
                    if (selected == 18) {
                        opt::visual::textcolor = "`q";
                    }
                    if (selected == 19) {
                        opt::visual::textcolor = "`e";
                    }
                    if (selected == 20) {
                        opt::visual::textcolor = "`r";
                    }
                    if (selected == 21) {
                        opt::visual::textcolor = "`t";
                    }
                    if (selected == 22) {
                        opt::visual::textcolor = "`a";
                    }
                    if (selected == 23) {
                        opt::visual::textcolor = "`s";
                    }
                    if (selected == 24) {
                        opt::visual::textcolor = "`c";
                    }

                }
                ImGui::EndChild();
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
    }
}