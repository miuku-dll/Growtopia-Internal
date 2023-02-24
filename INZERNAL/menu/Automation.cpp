#pragma once
#include <menu/menu.h>
#include <sdk/sdk.h>
//#include <proton\VariantDB.h>

void menu::Automation() {
	auto draw = ImGui::GetBackgroundDrawList();
    if (ImGui::BeginTabBar("##automations", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Auto 1")) {
            ImGui::BeginChild("Auto 1");
            if (ImGui::BeginChild("###autofarm", AUTOSIZEC(3.3), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("BFG Auto Farm");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Enable   ", &opt::cheat::autofarm);
                ImGui::SameLine();
                ImGui::Checkbox("Auto Stop Farm If Pulled", &opt::cheat::auto_stop_farm);
                ImGui::Text("Auto Farm ID: ");
                ImGui::SameLine();
                ImGui::InputInt("##Farmm", &opt::cheat::itemid_val);
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###autocollect", AUTOSIZEC(4), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Auto Collect");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Enable   ", &opt::cheat::autocollect);
                ImGui::SameLine();
                ImGui::SliderInt("##Range", &opt::cheat::range_val, 1, 10);
                ImGui::Checkbox("ItemFilter   ", &opt::cheat::filterautocollect);
                ImGui::SameLine();
                ImGui::InputInt("##ITEMID", &opt::cheat::itemfilter);
                //ImGui::Checkbox("ItemESP", &opt::cheat::itemesp);
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###spam", AUTOSIZEC(4), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Spammer");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Enable   ", &opt::cheat::spammer);
                ImGui::SameLine();
                ImGui::InputTextWithHint("##spam", "Spam text", opt::cheat::spam_text, IM_ARRAYSIZE(opt::cheat::spam_text));
                ImGui::Checkbox("Colored Text   ", &opt::cheat::coloredtext);
                ImGui::SameLine();
                ImGui::Checkbox("Anti Detect", &opt::cheat::antidetect);
                ImGui::Text("Interval: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
                ImGui::InputInt("##internval", &opt::cheat::spaminterval);
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###joinworld", AUTOSIZEC(3.3), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Join World Actions");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Auto Ban   ", &opt::cheat::auto_ban);
                ImGui::SameLine();
                ImGui::Checkbox("Auto Pull", &opt::cheat::auto_pull);
                ImGui::Checkbox("Auto Msg   ", &opt::cheat::auto_msg);
                ImGui::SameLine();
                ImGui::InputTextWithHint("##Msg", "Msg text", opt::cheat::msg_text, IM_ARRAYSIZE(opt::cheat::msg_text));
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###autoclear", AUTOSIZEC(6.6), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Auto Clear World");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Enable", &opt::cheat::autoclear);
                ImGui::Text("Punch Range: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
                ImGui::InputInt("##range", &opt::cheat::punchrange);
                ImGui::Text("Black Listed ItemID:");
                ImGui::InputInt("##bl1", &opt::cheat::bl1);
                ImGui::SameLine();
                ImGui::InputInt("##bl2", &opt::cheat::bl2);
                ImGui::SameLine();
                ImGui::InputInt("##bl3", &opt::cheat::bl3);
                ImGui::InputInt("##bl4", &opt::cheat::bl4);
                ImGui::SameLine();
                ImGui::InputInt("##bl5", &opt::cheat::bl5);
                ImGui::SameLine();
                ImGui::InputInt("##bl6", &opt::cheat::bl6);
                ImGui::InputInt("##bl7", &opt::cheat::bl7);
                ImGui::SameLine();
                ImGui::InputInt("##bl8", &opt::cheat::bl8);
                ImGui::SameLine();
                ImGui::InputInt("##bl9", &opt::cheat::bl9);
                ImGui::EndChild();
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Auto 2")) {
            ImGui::BeginChild("Auto 2");
            if (ImGui::BeginChild("###autosurg", AUTOSIZEC(2.2), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Auto Surgery");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Enable", &opt::cheat::autosurg);
                ImGui::EndChild();
            }
            if (ImGui::BeginChild("###role", AUTOSIZEC(2.2), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Auto Carnival");
                ImGui::EndMenuBar();
                ImGui::Checkbox("Enable   ", &opt::cheat::autocarnival);
                ImGui::SameLine();
                static const char* games[]{ "Growganoth Left", "Growganoth Right", "Death Race 5000", "Mirro Maze"};
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.30f);
                ImGui::Combo("##games", &opt::cheat::carnivalgame, games, IM_ARRAYSIZE(games));
                ImGui::EndChild();
            }
        }
        ImGui::EndChild();
        ImGui::EndTabItem();
    }

}
