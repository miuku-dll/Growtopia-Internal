#pragma once
#include <menu/menu.h>
#include <sdk/Consts.h>
#include <sdk/ResourceManager.h>
#include <sdk/sdk.h>


void menu::realtime_tab() {
    auto client = sdk::GetClient();
    auto logic = sdk::GetGameLogic();
    auto local = logic->GetLocalPlayer();
    auto world = logic->GetWorld();
    auto tilemap = logic->GetTileMap();

    if (ImGui::BeginTabBar("idk", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("RealTime 1")) {
            ImGui::BeginChild("RealTime 1");

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            static int ix = C_PUNCH_RELOAD_TIME;

            if (ImGui::BeginChild("###cooldownchanger", AUTOSIZEC(2.2), true, ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Punch speed changer");
                ImGui::EndMenuBar();
                if (ImGui::Checkbox("Enabled   ###punch", &opt::cheat::punch_cooldown_on)) {
                    if (!opt::cheat::punch_cooldown_on)
                        consts::set_float(ix, consts::defs[ix]);
                    else
                        consts::set_float(ix, opt::cheat::punch_cooldown_val);
                }
                ImGui::SameLine();
                if (ImGui::SliderFloat("###punchc", &opt::cheat::punch_cooldown_val, 0.0f, 0.4f, "%0.2f") && opt::cheat::punch_cooldown_on)
                    consts::set_float(ix, opt::cheat::punch_cooldown_val);

                ImGui::EndChild();
                ImGui::Spacing();
            }


            if (imwrap::fchanger("Gravity Changer", opt::cheat::gravity_val, opt::cheat::gravity_on, -500.f, 2000.f) && local)
                local->gravity.set(global::state.gravity);
            ImGui::Spacing();
            if (imwrap::fchanger("Acceleration changer", opt::cheat::accel_val, opt::cheat::accel_on, -500.f, 10000.f) && local)
                local->accel.set(global::state.accel);
            ImGui::Spacing();
            if (imwrap::fchanger("Speed changer", opt::cheat::speed_val, opt::cheat::speed_on, 100.f, 1400.f) && local)
                local->speed.set(global::state.speed);
            ImGui::Spacing();
            if (imwrap::fchanger("Water Speed changer", opt::cheat::waterspeed_val, opt::cheat::waterspeed_on, 0.f, 1400.f) && local)
                local->water_speed = global::state.water_speed;

            ImGui::PopStyleVar();


            //dont worry bringing back instant stop and start later on without the hooks.
            ImGui::EndChild();
            ImGui::EndTabItem();

        }

        if (ImGui::BeginTabItem("RealTime 2")) {
            ImGui::BeginChild("RealTime 2");
            static bool isConstOn = true;


            if (ImGui::BeginChild("###Constants", isConstOn ? AUTOSIZEC(22.6) : AUTOSIZEC(4), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar)) {
                ImGui::BeginMenuBar();
                ImGui::Text("Game constants");
                ImGui::EndMenuBar();
                int i = 0;
                if (ImGui::Button("Global Reset")) {
                    consts::reset_all();
                    if (opt::cheat::punch_cooldown_on) //dont reset this if we have the other option on for this
                        consts::set_float(C_PUNCH_RELOAD_TIME, opt::cheat::punch_cooldown_val);
                }
                ImGui::Spacing();
                imwrap::const_slider("Max fall speed", 0.f, 2000.f, "%.0f", i++, "Pretty simple");
                imwrap::const_slider("Float power", 0.f, 10000.f, "%.0f", i++, "Higher makes damage flying curve smaller");
                imwrap::const_slider("Wind speed", 0.f, 2000.0f, "%.0f", i++, "The speed winter wind and others push you");
                imwrap::const_slider("Wind speed (against)", 0.f, 200.0f, "%.0f", i++, "The resistance of going against winter wind");
                imwrap::const_slider("Coll. max divergence", 0.f, 128.0f, "%.0f", i++, "No clue what this does, no references, no effect");
                imwrap::const_slider("Player drag", -500.f, 2400.0f, "%.0f", i++, "Drag for walking. Low values allow 'moonwalk'.");
                imwrap::const_slider("Initial jump boost", -100.f, -1000.0f, "%.0f", i++, "Affects how high you can jump in a different way from gravity");
                imwrap::const_slider(
                    "Control hurt mod", 0.0f, 1.5f, "%0.2f", i++, "Affects how well you are able to control your char when hurt. 1 = normal, 0 = no control.");
                imwrap::const_slider("Lava bounce [x]", -100.f, 1000.0f, "%.0f", i++, "Just how much you bounce from lava/etc horizontally");
                imwrap::const_slider("Lava bounce [y]", -100.f, 1400.0f, "%.0f", i++, "Just how much you bounce from lava/etc vertically");
                imwrap::const_slider("Trampoline bounce", -100.f, 1600.0f, "%.0f", i++, "Bounciness for mushroom/etc");
                imwrap::const_slider("Regular bounce", -100.f, 1600.0f, "%.0f", i++, "Bounciness for pinballs/etc");
                imwrap::const_slider("Def. punch knockback", -500.f, 500.0f, "%.0f", i++, "Does not seem to have much effect except for anims (clientside)");
                imwrap::const_slider("Parasol gravity mod", 0.f, 1.0f, "%0.2f", i++, "How much parasol and similar affect gravity");
                imwrap::const_slider("Water gravity mod", 0.f, 2.0f, "%0.2f", i++, "How much water affects gravity");
                imwrap::const_slider("Mars gravity mod", 0.f, 2.0f, "%0.2f", i++, "How much mars affects gravity");
                imwrap::const_slider("Engine speedhack", 0.f, 4000.0f, "%.0f", i++, "Game engine speedhack. Affects various things, some need re-enter to apply fully.");
                imwrap::const_slider("Ghost speed", 0.f, 1000.0f, "%.0f", i++, "When you have the /ghost playmod, not related to ghosts that slime you.");
                imwrap::const_slider(
                    "Water lev. time", 0.f, 6000.0f, "%.0f", i++, "Probably the time that the clothes that keep you floating on water have any effect. Didn't test.");
                imwrap::const_slider("Hack report delay (ms)", 100.f, 30000.0f, "%.0f", i++, "For example cheat engine open report, etc. Shouldn't have much of any use");

            }
            ImGui::EndChild();


            ImGui::EndChild();
            ImGui::EndTabItem();

        }
    }
}