#pragma once
#include <menu/menu.h>
#include <sdk/Consts.h>
#include <sdk/ResourceManager.h>
#include <sdk/sdk.h>
#include <hooks/SendPacket.h>
#include <hooks/SendPacketRaw.h>
#include "sdk/world/WorldCamera.h"
#include <d3d/DrawingLUL.h>
#include "core/globals.h"
#include "proton/Math/rtRect.h"
#include <hooks/ProcessTankUpdatePacket.h>
#include <Keyauth/Requests.hpp>


void menu::cheats_tab() {
    auto logic = sdk::GetGameLogic();
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto locals = sdk::GetGameLogic()->GetNetObjMgr();
    auto tilemap = logic->GetTileMap();
    auto object = logic->GetObjectMap();
    auto w2s = logic->GetWorldRenderer()->camera;
    auto pos2f = local->GetPos();
    auto draw = ImGui::GetBackgroundDrawList();

    if (ImGui::BeginTabBar("Cheats", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Cheats")) {
            ImGui::BeginChild("Cheat 1");
            if (ImGui::BeginChild("###cheats", AUTOSIZEC(8.7), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Random Tools");
                ImGui::EndMenuBar();
                ImGui::Columns(2, nullptr, false);
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
                ImGui::Checkbox("Brute Force Door   ", &opt::cheat::bruteforcepassdoor);
                ImGui::SameLine();
                ImGui::InputInt("##StartFrom", &opt::cheat::brutestart);
                ImGui::Text("Format: ");
                ImGui::SameLine();
                ImGui::InputText("##format", opt::cheat::bruteform,  CHAR_MAX);
                ImGui::Checkbox("NPC Mode", &opt::cheat::dancemove);
                ImGui::Checkbox("Anti Ghost", &opt::cheat::antighost);
                ImGui::Checkbox("Anti Punch", &opt::cheat::antipunch);
                ImGui::Checkbox("Block SPR", &opt::cheat::block_sendpacketraw);
                ImGui::Checkbox("Mod Zoom", &opt::cheat::mod_zoom);
                if (ImGui::Button("Invis")) {
                    std::string packetC = "action|setDeath\nanimDeath|1";
                    SendPacketHook::Execute(2, packetC, sdk::GetPeer());
                    GameUpdatePacket packet{ 0 };
                    packet.flags = 2308;
                    gt::send(&packet);
                    opt::cheat::dancemove = true;
                }
                ImGui::SameLine();
                if (ImGui::Button("Fake Respawn")) {
                    GameUpdatePacket packet{ 0 };
                    packet.flags = 2308;
                    gt::send(&packet);
                }
                ImGui::NextColumn();
                ImGui::Checkbox("Hide Names", &opt::cheat::noname);
                ImGui::Checkbox("Click All Roulette", &opt::cheat::clickallroulette);
                ImGui::Checkbox("Dev Zoom", &opt::cheat::dev_zoom);
                ImGui::Checkbox("Epilepsy", &opt::cheat::blinkcolor);
                ImGui::Checkbox("Local building", &sdk::GetGameLogic()->local_building);
                ImGui::Checkbox("Anti Collect", &opt::cheat::anticollect);
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###serversided", AUTOSIZEC(9.6), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Trolling Tools");
                ImGui::EndMenuBar();
                ImGui::Columns(2, nullptr, false);
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
                ImGui::Checkbox("Get Netid   ", &opt::cheat::GetNetid);
                ImGui::SameLine();
                ImGui::InputInt("##netid", &opt::cheat::GetNetids);
                ImGui::Checkbox("Spam Trade", &opt::cheat::spamtrade);
                ImGui::Checkbox("Kill Aura", &opt::cheat::killaura);
                ImGui::Checkbox("Netid Nazi", &opt::cheat::netidnazi);
                ImGui::Checkbox("Pf Around Netid", &opt::cheat::pfaround);
                ImGui::Checkbox("Punch Everyone", &opt::cheat::punchaura);
                ImGui::Checkbox("Free Tile Punch", &opt::cheat::freetilepunch);
                ImGui::Checkbox("Self Punch", &opt::cheat::selfpunch);
                ImGui::Checkbox("Spam NetID   ", &opt::cheat::spamnetid);
                ImGui::SameLine();
                ImGui::InputInt("##netiditem", &opt::cheat::spamnetiditem);
                ImGui::NextColumn();
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
                ImGui::Checkbox("Follow Player", &opt::cheat::followplayer);
                ImGui::Checkbox("State Spammer   ", &opt::cheat::spamwater);
                ImGui::SameLine();
                ImGui::InputInt("##id", &opt::cheat::watervalue);
                ImGui::Checkbox("Crystal Only", &opt::cheat::crystalmode);
                ImGui::Checkbox("Random Value", &opt::cheat::randommodepacketstate);
                ImGui::Checkbox("Respawn Spam", &opt::cheat::resspam);
                ImGui::Checkbox("Cactus Spam", &opt::cheat::spamcactus);
                ImGui::Checkbox("Jump Spam", &opt::cheat::jumpspam);
                ImGui::Checkbox("Bubble Spam", &opt::cheat::bubblespam);
                ImGui::Checkbox("Freetile Spam   ", &opt::cheat::freetilespam);
                ImGui::SameLine();
                ImGui::InputInt("##freetileitem", &opt::cheat::freetilespamid);
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###pathmarker", AUTOSIZEC(4), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Pathmarker Exploit");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Enable", &opt::cheat::pathmarkerexploit);
                ImGui::Checkbox("On Current World", &opt::cheat::pathmarkerexploitlocalworld);
                /*if (ImGui::Checkbox("On Other World", &opt::cheat::pathmarkerexploitotherworld)); {
                    ImGui::SameLine();
                    ImGui::InputText("##pmworld", opt::cheat::pathmarkerworld, CHAR_MAX);
                }*/
                ImGui::Text("Pathmarker ID");
                ImGui::SameLine();
                ImGui::InputText("##pmtxt", opt::cheat::pathmarkertxt, CHAR_MAX);
                ImGui::EndChild();
            }

            ImGui::EndChild();
            ImGui::EndTabItem();

        }
        if (ImGui::BeginTabItem("Even more cheats")) {
            ImGui::BeginChild("Cheat 2");
            if (ImGui::BeginChild("###fastthings", AUTOSIZEC(6.8), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("QOL Tools");
                ImGui::EndMenuBar();
                ImGui::Columns(2, nullptr, false);
                ImGui::Checkbox("Fast Empty Donation Box", &opt::cheat::emptybox);
                ImGui::Checkbox("Fast Add Stock To Vend", &opt::cheat::addstock);
                ImGui::Checkbox("Fast Empty Vend", &opt::cheat::pullstock);
                ImGui::Checkbox("Fast Buy Vend", &opt::cheat::fastvend);
                ImGui::Checkbox("Fast Drop", &opt::cheat::fastdrop);
                ImGui::Checkbox("Fast trash", &opt::cheat::fasttrash);
                ImGui::NextColumn();
                ImGui::Checkbox("Title Pos", &opt::cheat::titlepos);
                ImGui::Checkbox("Auto Go When Giveaway", &opt::cheat::autogiveaway);
                ImGui::Checkbox("Spin Checker", &opt::cheat::spin_checker);
                ImGui::Checkbox("Owner / Admin Detect", &opt::cheat::owner_detect);
                ImGui::Checkbox("Fast Sign   ", &opt::cheat::fastsign);
                ImGui::SameLine();
                ImGui::InputText("##fstxt", opt::cheat::fastsigntxt, CHAR_MAX);
                ImGui::Checkbox("Fast Billboard   ", &opt::cheat::fastboard);
                ImGui::SameLine();
                ImGui::InputText("##fsadtxt", opt::cheat::fastboardtxt, CHAR_MAX);
                ImGui::EndChild();
            }
            ImGui::BeginChild("Opcode");
            if (ImGui::BeginChild("###opcodes", AUTOSIZEC(5), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Character Mods");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Moderator Fly", &opt::cheat::modfly);
                ImGui::Checkbox("Ghost", &opt::cheat::ghost);
                /*static bool seeinchest = false;
                if (seeinchest) { //+8C69A7
                    uintptr_t ad_seeinchest = 0x9E4B4B;
                    if (utils::patch_bytes<6>(global::gt + ad_seeinchest, "\x90\x90\x90\x90\x90\x90"));
                }
                if (!seeinchest) {
                    uintptr_t ad_seeinchest = 0x9E4B4B;
                    if (utils::patch_bytes<6>(global::gt + ad_seeinchest, "\x0F\x82\xDE\x19\x00\x00"));
                }
                ImGui::Checkbox("See Inside Chest", &seeinchest);


                if (opt::cheat::antibounce) {
                    uintptr_t ad_antibounce = 0x9D4D9A;
                    if (utils::patch_bytes<6>(global::gt + ad_antibounce, "\xE9\x04\x01\x00\x00\x90"));
                    uintptr_t ad_antiboucenbypass = 0x859335;
                    if (utils::patch_bytes<4>(global::gt + ad_antiboucenbypass, "\x90\x90\x90\x90"));

                }
                if (!opt::cheat::antibounce) {
                    uintptr_t ad_antibounce = 0x9D4D9A;
                    if (utils::patch_bytes<6>(global::gt + ad_antibounce, "\x0F\x84\x03\x01\x00\x00"));
                    uintptr_t ad_antiboucenbypass = 0x859335; // 83 4B 0C 20
                    if (utils::patch_bytes<4>(global::gt + ad_antiboucenbypass, "\x83\x4B\x0C\x20"));

                }
                ImGui::Checkbox("Anti Bounce", &opt::cheat::antibounce);
                ImGui::Checkbox("Anti Slide", &opt::cheat::antislide);
                */
                if (ImGui::Button("Growscan")) {
                    std::string paket;
                    paket =
                        "\nadd_label_with_icon|big|`2Growscan|left|6016|"
                        "\nadd_button|gfloat|`92loating items``|noflags|0|0|"
                        "\nend_dialog|end|Cancel|Okay|";
                    variantlist_t liste{ "OnDialogRequest" };
                    liste[1] = paket;
                    gt::send_varlist_self(liste);
                }

            }

            ImGui::EndChild();
            /*if (ImGui::BeginChild("###weather", AUTOSIZEC(3.1), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Weather Tools");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Enable   ", &opt::cheat::weatheresp);
                ImGui::SameLine();
                ImGui::Checkbox("Punch Weather When Close", &opt::cheat::weatherpunch);
                ImGui::Checkbox("PathFind To Weather", &opt::cheat::weatherpf);   

                ImGui::EndChild(); 
            }*/
            //if (ImGui::BeginChild("###esptools", AUTOSIZEC(3.1), true, ImGuiWindowFlags_MenuBar)) {
              //  ImGui::BeginMenuBar();
                //ImGui::Text("ESP Tools");
                //ImGui::EndMenuBar();
                //ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
                //ImGui::Checkbox("Block ESP   ", &opt::cheat::customblockesp);
                //ImGui::SameLine();
                //ImGui::InputInt("##id", &opt::cheat::customblockespid);
                //ImGui::Checkbox("Floating Item ESP   ", &opt::cheat::customitemesp);
                //ImGui::SameLine();
                //ImGui::InputInt("##idd", &opt::cheat::customitemespid);
                //ImGui::EndChild();
            //}
            if (ImGui::BeginChild("###destroy", AUTOSIZEC(4), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Search And Destroy");
                ImGui::EndMenuBar();
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
                ImGui::Checkbox("Enable", &opt::cheat::autodestroy);
                ImGui::Text("Foreground: ");
                ImGui::SameLine();
                ImGui::InputInt("##id1", &opt::cheat::autodestroyfid);
                ImGui::Text("Background: ");
                ImGui::SameLine();
                ImGui::InputInt("##id2", &opt::cheat::autodestroybid);
                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }

        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}