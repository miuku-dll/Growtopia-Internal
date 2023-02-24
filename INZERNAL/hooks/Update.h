#pragma once
#include <core/globals.h>
#include <core/gt.h>
#include <hooks/hooks.h>
#include <sdk/Consts.h>

static bool NoMoreUpdating;
class UpdateManager {
   public:
    //for code that needs to be handled on injection as well
    static void OnInject(App* app) {
        static auto orig_fps = decltype(&hooks::BaseApp_SetFPSLimit)(hookmgr->orig(sig::baseapp_setfpslimit));
        orig_fps((BaseApp*)app, opt::fps_limit);
        utils::printc("93", "Modified FPS limit!");
        global::app = app;
        utils::unprotect_process();

        consts::set_defaults();

        if (opt::cheat::punch_cooldown_on)
            consts::set_float(C_PUNCH_RELOAD_TIME, opt::cheat::punch_cooldown_val);

        auto gamelogic = sdk::GetGameLogic();
        if (!gamelogic)
            return;
        auto player = gamelogic->GetLocalPlayer();
        if (!player)
            return;

        global::state.copy_inject(player, true);
        player->SetModStatus(opt::cheat::mod_zoom, opt::cheat::dev_zoom);
        player->SetCharacterMods(opt::cheat::dash, opt::cheat::jump_charge, opt::cheat::jump_cancel);
    }
    static void OnDestroy() {
        NoMoreUpdating = true;
        auto app = global::app;

        consts::reset_all(); //reset all consts

        auto gamelogic = sdk::GetGameLogic();
        if (!gamelogic)
            return;
        auto local = gamelogic->GetLocalPlayer();
        if (!local)
            return;

        local->SetModStatus(false, false);
        local->SetCharacterMods(false, false, false);

        if (opt::cheat::gravity_on)
            local->gravity.set(global::state.gravity);
        if (opt::cheat::accel_on)
            local->accel.set(global::state.accel);
        if (opt::cheat::speed_on)
            local->speed.set(global::state.speed);
        if (opt::cheat::waterspeed_on)
            local->water_speed = global::state.water_speed;
    }

    static void OnJoinedWorld(NetAvatar* local) {



        if (!opt::spoof_once && !opt::cheat::followplayer)
        {
            std::string packet1;
            std::string packet2;
            std::string packet3;
            std::string packet4;
            packet1 =
                "spawn|avatar"
                "\nnetID|9999"
                "\nuserID|99999"
                 "\ncolrect|0|0|20|30";
            packet2 =
                "\nposXY|" + std::to_string(local->pos.x) + "|" + std::to_string(local->pos.y);
            packet3 =
                "\nname|````````"
                "\ncountry|" + local->country;
            packet4 =
                "\ninvis|0"
                "\nmstate|0"
                "\nsmstate|0"
                "\nonlineID|"
                "\ntype|";
            variantlist_t varlst2{ "OnSpawn" };
            varlst2[1] = packet1 + packet2 + packet3 + packet4;
            gt::send_varlist_self(varlst2);
            opt::spoof_once = true;
        }
        else {
            opt::spoof_once = false;
        }
        if (opt::visual::weather) {
            variantlist_t varlist{ "OnSetCurrentWeather" };
            varlist[1] = opt::visual::weatherint;
            gt::send_varlist_self(varlist);
        }
        if (opt::visual::clothes) {
            variantlist_t vlist{ "OnSetClothing" };
            vlist[1] = vector3_t{ (float)opt::visual::vitemidX1, (float)opt::visual::vitemidY1, (float)opt::visual::vitemidZ1 };
            vlist[2] = vector3_t{ (float)opt::visual::vitemidX2, (float)opt::visual::vitemidY2, (float)opt::visual::vitemidZ2 };
            vlist[3] = vector3_t{ (float)opt::visual::vitemidX3, (float)opt::visual::vitemidY3, (float)opt::visual::vitemidZ3 };
            vlist[4] = opt::visual::skincolor;
            vlist[5] = vector3_t{ (float)opt::visual::vitemidX5, 1, 0 };
            gt::send_varlist_self(vlist, sdk::GetGameLogic()->GetLocalPlayer()->netid);
        }
        if (opt::visual::name) {
            variantlist_t varlist{ "OnNameChanged" };
            varlist[1] = sdk::GetGameLogic()->GetLocalPlayer()->name = opt::visual::nametxt;
            gt::send_varlist_self(varlist, sdk::GetGameLogic()->GetLocalPlayer()->netid);
        }
        if (opt::visual::title && opt::visual::titletxt != "default") {
            std::string packetmaster = local->country + opt::visual::titletxt;
            variantlist_t packet123{ "OnCountryState" };
            packet123[1] = packetmaster.c_str();
            gt::send_varlist_self(packet123, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
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
            variantlist_t vlist{ "OnSetRoleSkinsAndIcons" };
            vlist[1] = opt::visual::role;
            vlist[2] = opt::visual::role;
            gt::send_varlist_self(vlist, sdk::GetGameLogic()->GetLocalPlayer()->netid, -1);
        }
        auto t = sdk::GetGameLogic()->GetTileMap();
        for (auto& tile : t->tiles)
        {
            if (tile.position_x == 99 && tile.position_y == 55 && tile.foreground == 8) {
                tile.foreground = 4992;
                tile.flags = 64;
            }
            if (opt::cheat::antibounce)
            {
                if (tile.foreground == 4) {
                    tile.foreground = 8;
                }
                if (tile.foreground == 1630) {
                    tile.foreground = 8;
                }
                if (tile.foreground == 1128) {
                    tile.foreground = 8;
                }
                if (tile.foreground == 726) {
                    tile.foreground = 8;
                }
                if (tile.foreground == 526) {
                    tile.foreground = 8;
                }
                if (tile.foreground == 7376) {
                    tile.foreground = 8;
                }
                if (tile.foreground == 3582) {
                    tile.foreground = 8;
                }
                if (tile.foreground == 3584) {
                    tile.foreground = 8;
                }
            }
            if (opt::cheat::antislide)
            {
                if (tile.foreground == 1344) {
                    tile.foreground = 18;
                }
                if (tile.foreground == 440) {
                    tile.foreground = 18;
                }
                if (tile.foreground == 978) {
                    tile.foreground = 18;
                }
            }
        }
        local->SetModStatus(opt::cheat::mod_zoom, opt::cheat::dev_zoom);
        local->SetCharacterMods(opt::cheat::dash, opt::cheat::jump_charge, opt::cheat::jump_cancel);
    }
    static void OnUpdateInWorld(App* app, NetAvatar* local) {

        if (opt::cheat::gravity_on) {
            if (opt::cheat::gravity_val != local->gravity.get())
                local->gravity.set(opt::cheat::gravity_val);
        }
        if (opt::cheat::accel_on) {
            if (opt::cheat::accel_val != local->accel.get())
                local->accel.set(opt::cheat::accel_val);
        }
        if (opt::cheat::speed_on) {
            if (opt::cheat::speed_val != local->speed.get())
                local->speed.set(opt::cheat::speed_val);
        }
        if (opt::cheat::waterspeed_on) {
            if (opt::cheat::waterspeed_val != local->water_speed)
                local->water_speed = opt::cheat::waterspeed_val;
        }

    }
    //any kind of continuous code can be ran here
    static void Execute(App* app) {
        static auto orig = decltype(&hooks::App_Update)(hookmgr->orig(sig::app_update));
        if (!global::app && app)
            OnInject(app);

        if (!global::d9init)
            hooks::init_endscene();

        auto gamelogic = sdk::GetGameLogic();
        if (gamelogic && !NoMoreUpdating) {
            auto player = gamelogic->GetLocalPlayer();
            if (player)
                OnUpdateInWorld(app, player);

        }
        
        orig(app);
    }
};