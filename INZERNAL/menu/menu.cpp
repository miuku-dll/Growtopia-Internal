#pragma once
#include <core/utils.h>
#include <menu/menu.h>
#include "core/globals.h"
#include "proton/Math/rtRect.h"
#include <sdk/sdk.h>
#include <hooks/hooks.h>
#include <hooks/SendPacketRaw.h>
#include <hooks/SendPacket.h>
#include<hooks/ProcessTankUpdatePacket.h>
#include <thread>
#include <chrono>
#include <d3d/tab1.h>
#include<menu/icons.h>
#include<PathFinder/Astar.hpp>

int rmode = 1;
int firs = 255;
int seco = 0;
int thir = 0;
ImColor ESPColor;
void menu::animate() {
    static bool direction = false;

    //most imgui controls have a custom added fading effect
    global::fade += direction ? -0.003f : 0.003f;
    if (global::fade <= 0.0f) {
        direction = !direction;
        global::fade = 0.0f;
    }
    else if (global::fade >= 0.3f) {
        direction = !direction;
        global::fade = 0.3f;
    }
}
void doPftoitem() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto logic = sdk::GetGameLogic();
    auto objmap = logic->GetObjectMap();
    auto item = objmap->objects.begin();
    if (local) {
        for (auto it = objmap->objects.begin(); it != objmap->objects.end(); ++it) {
            if (item->item_id == 1458) {
                gt::pathFindTo(item->pos.x / 32, item->pos.y / 32);
            }
        }

    }

}
void doResspam() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto x = local->tile_x;
    auto y = local->tile_y;
    static types::time time = std::chrono::system_clock::now();
    if (utils::run_at_interval(time, 0.1f)) {
        std::string packet = "action|respawn_spike\ntileX|" + std::to_string(x) + "\ntileY|" + std::to_string(y);
        gt::send(2, packet);
    }
}
void doBubblespam() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    GameUpdatePacket packet{ 0 };
    packet.type = 0;
    static types::time timer = std::chrono::system_clock::now();
    static types::time times = std::chrono::system_clock::now();
    static int poggers;
    std::vector<int> flags;
    flags = { 32, 48 };
    if (utils::run_at_interval(timer, 0.5)) {
        poggers = flags.at(rand() % 2);
    }
    packet.flags = poggers;
    packet.int_x = local->pos.x;
    packet.int_y = local->pos.y;
    if (utils::run_at_interval(times, 0.025)) {
        SendPacketRawHook::Execute(4, &packet, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
    }
}

void doModfly() {
    bool modfly = false;
    auto offset_modfly = sigs::get(sig::modfly);

    if (opt::cheat::modfly && !modfly && !(GetAsyncKeyState('S'))) {
        utils::patch_bytes<2>(offset_modfly, "\x90\x90");
        modfly = true;
    }
    else if (opt::cheat::enter == false && GetKeyState('S')) {
        utils::patch_bytes<2>(offset_modfly, "\x74\x5D");
        modfly = false;
    }

    if (!opt::cheat::modfly)
        utils::patch_bytes<2>(offset_modfly, "\x74\x5D");

}
void doBruteforce() {
    auto logic = sdk::GetGameLogic();
    auto tilemap = logic->GetTileMap();
    static types::time time = std::chrono::system_clock::now();
    if (utils::run_at_interval(time, 0.015f)) {
        auto local = sdk::GetGameLogic()->GetLocalPlayer();
        if (local) {
            Tile* tile = tilemap->GetTileSafe(local->GetPos() / 32);
            if (tile) {
                if (tile->foreground == 762 || tile->foreground == 8260) {
                    opt::cheat::brutestart++;
                    std::string loz = std::to_string(opt::cheat::brutestart);
                    std::string asd = loz + opt::cheat::bruteform;
                    GameUpdatePacket packet{ 0 };
                    packet.type = PACKET_TILE_ACTIVATE_REQUEST;
                    packet.int_x = local->pos.x / 32;
                    packet.int_y = local->pos.y / 32;
                    SendPacketRawHook::Execute(4, &packet, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                    int tilex, tiley;
                    tilex = local->pos.x / 32;
                    tiley = local->pos.y / 32;
                    std::string packets = "action|dialog_return\ndialog_name|password_reply\ntilex|" + std::to_string(tilex) + "|\ntiley|" + std::to_string(tiley) +
                        "|\npassword|" + (opt::cheat::bruteform, std::to_string(opt::cheat::brutestart));
                    SendPacketHook::Execute(2, packets, sdk::GetPeer());
                }
                else {
                    gt::sendlog("`7The Pass is about: `b" + std::to_string(opt::cheat::brutestart - 1));
                    opt::cheat::bruteforcepassdoor = false;
                    opt::cheat::brutestart = 0;
                }
            }
        }
        else {
            opt::cheat::bruteforcepassdoor = false;
            opt::cheat::brutestart = 0;
        }
    }
}
void doSpamParticle() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    static types::time times = std::chrono::system_clock::now();
    if (utils::run_at_interval(times, 0.025)) {
        variantlist_t varlst{ "OnParticleEffect" };
        varlst[1] = opt::visual::particleint;
        varlst[2] = vector2_t{ local->pos.x + 10, local->pos.y + 15 };
        varlst[3] = 0;
        varlst[4] = 0;
        gt::send_varlist_self(varlst);
    }
}
void autocollect() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        auto draw = ImGui::GetBackgroundDrawList();
        auto logic = sdk::GetGameLogic();
        auto tilemap = logic->GetTileMap();
        auto pos2f = local->GetPos();
        auto objmap = logic->GetObjectMap();
        auto rend = sdk::GetGameLogic()->renderer;

        CL_Vec2f localposfirst;
        localposfirst.x = local->GetPos().x + opt::cheat::range_val * 32 + 10;
        localposfirst.y = local->GetPos().y + opt::cheat::range_val * 32 + 15;
        auto LocalScreenFirst = rend->GetCamera()->WorldToScreen(localposfirst);
        CL_Vec2f localpossec;
        localpossec.x = local->GetPos().x - opt::cheat::range_val * 32 + 10;
        localpossec.y = local->GetPos().y - opt::cheat::range_val * 32 + 15;
        auto LocalScreenSec = rend->GetCamera()->WorldToScreen(localpossec);
        draw->AddRect(ImVec2(LocalScreenFirst.x, LocalScreenFirst.y), ImVec2(LocalScreenSec.x, LocalScreenSec.y), ESPColor, 1000.0f,
            0, 2.0f);
        float tilex = pos2f.x / 32;
        float tiley = pos2f.y / 32;
        for (auto it = objmap->objects.begin(); it != objmap->objects.end(); ++it) {
            if (utils::isInside(it->pos.x, it->pos.y, opt::cheat::range_val * 32 + 12, pos2f.x, pos2f.y)) {
                static types::time times = std::chrono::system_clock::now();
                auto mf = it->pos;
                float mf1 = mf.x;
                float mf2 = mf.y;
                Tile* tile = tilemap->GetTileSafe(mf1 / 32, mf2 / 32);
                if (opt::cheat::filterautocollect) {
                    if (it->item_id == opt::cheat::itemfilter) {
                        GameUpdatePacket packet{ 0 };
                        packet.pos_x = it->pos.x;
                        packet.pos_y = it->pos.y;
                        packet.type = 11;
                        packet.netid = -1;
                        packet.object_id = it->object_id;

                        SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                    }
                }
                else {
                    GameUpdatePacket packet{ 0 };
                    packet.pos_x = it->pos.x;
                    packet.pos_y = it->pos.y;
                    packet.type = 11;
                    packet.netid = -1;
                    packet.object_id = it->object_id;

                    SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                }
            }
        }
    }
}

void doAutofarm() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        auto logic = sdk::GetGameLogic();
        auto tilemap = logic->GetTileMap();
        bool isLeft = false;
        if (local->flags = local->facing_left) {
            isLeft = true;
        }
        auto tile = local->GetPos() / 32;
        static types::time timerlol = std::chrono::system_clock::now();
        static int mfoffsets = isLeft ? -1 : 1;
        if (utils::run_at_interval(timerlol, 0.20)) {
            mfoffsets += isLeft ? -1 : 1;
            if (mfoffsets > 2 || mfoffsets < -2)
                mfoffsets = isLeft ? -1 : 1;
        }
        Tile* tilepog = tilemap->GetTileSafe(tile.x + mfoffsets, tile.y);


        GameUpdatePacket legitpacket{ 0 };
        legitpacket.type = PACKET_STATE;
        legitpacket.vec_x = local->pos.x;
        legitpacket.vec_y = local->pos.y;

        if (tilepog->background == 0 && tilepog->foreground == 0) {
            GameUpdatePacket placepacket{ 0 };
            placepacket.type = PACKET_TILE_CHANGE_REQUEST; // packet 3
            placepacket.int_data = opt::cheat::itemid_val; // magplant remote id
            placepacket.vec_x = local->GetPos().x;
            placepacket.vec_y = local->GetPos().y;
            placepacket.int_x = tilepog->position_x;
            placepacket.int_y = tilepog->position_y;

            legitpacket.int_data = opt::cheat::itemid_val;
            legitpacket.int_x = tilepog->position_x;
            legitpacket.int_y = tilepog->position_y;
            legitpacket.flags = (1 << 5) | (1 << 10) | (1 << 11);

            static types::time time = std::chrono::system_clock::now();
            if (utils::run_at_interval(time, 0.155)) {
                gt::send(&legitpacket);
                SendPacketRawHook::Execute(4, &placepacket, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
            }
        }
        else {
            GameUpdatePacket punchpacket{ 0 };
            punchpacket.type = PACKET_TILE_CHANGE_REQUEST; // packet 3
            punchpacket.int_data = 18;                     // punch id
            punchpacket.vec_x = local->GetPos().x;
            punchpacket.vec_y = local->GetPos().y;
            punchpacket.int_x = tilepog->position_x;
            punchpacket.int_y = tilepog->position_y;

            legitpacket.int_data = 18;
            legitpacket.int_x = tilepog->position_x;
            legitpacket.int_y = tilepog->position_y;
            legitpacket.flags = 2560;

            static types::time timepunch = std::chrono::system_clock::now();
            if (utils::run_at_interval(timepunch, 0.155)) {
                gt::send(&legitpacket);
                SendPacketRawHook::Execute(4, &punchpacket, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
            }
        }
        auto draw = ImGui::GetBackgroundDrawList();
        auto rend = sdk::GetGameLogic()->renderer;
        CL_Vec2f blocktop;
        blocktop.x = tilepog->position_x * 32;
        blocktop.y = tilepog->position_y * 32;
        auto screenBlockTOP = rend->GetCamera()->WorldToScreen(blocktop);
        CL_Vec2f blockbottom;
        blockbottom.x = tilepog->position_x * 32;
        blockbottom.y = tilepog->position_y * 32;
        blockbottom.x = blockbottom.x + 32;
        blockbottom.y = blockbottom.y + 32;
        auto screenBlockBottom = rend->GetCamera()->WorldToScreen(blockbottom);
        draw->AddRectFilled(ImVec2(screenBlockTOP.x, screenBlockTOP.y), ImVec2(screenBlockBottom.x, screenBlockBottom.y), ImColor(255, 0, 0, 100));
    }
    else {
        opt::cheat::autofarm = false;
    }
}

void playeresp() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        auto draw = ImGui::GetBackgroundDrawList();

        auto locals = sdk::GetGameLogic()->GetNetObjMgr();
        auto logic = sdk::GetGameLogic();
        auto pos2f = local->GetPos();
        auto objmap = logic->GetObjectMap();
        auto tilemap = logic->GetTileMap();
        auto rend = sdk::GetGameLogic()->renderer;

        float tilex = pos2f.x / 32;
        float tiley = pos2f.y / 32;
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
#pragma region shit lot
            std::string name = pair.second->name;
            const char* namechar = name.c_str();
            CL_Vec2f player;
            player.x = pair.second->pos.x - 8;
            player.y = pair.second->pos.y - 8;
            auto ScreenPlayer = rend->GetCamera()->WorldToScreen(player);
            CL_Vec2f playerline;
            playerline.x = pair.second->pos.x + 10;
            playerline.y = pair.second->pos.y - 8;
            auto ScreenPlayerLine = rend->GetCamera()->WorldToScreen(playerline);
            CL_Vec2f playername;
            playername.x = pair.second->pos.x + 5;
            playername.y = pair.second->pos.y + 30;
            auto PlayerNameScreen = rend->GetCamera()->WorldToScreen(playername);
            CL_Vec2f LocalPlayer;
            LocalPlayer.x = local->GetPos().x + 10;
            LocalPlayer.y = local->GetPos().y - (local->GetSize().y / 2);
            auto ScreenLocal = rend->GetCamera()->WorldToScreen(LocalPlayer);
            CL_Vec2f foot;
            foot.x = pair.second->pos.x + pair.second->GetSize().x + 5;
            foot.y = pair.second->pos.y + pair.second->GetSize().y + 5;
            auto footcam = rend->GetCamera()->WorldToScreen(foot);
#pragma endregion
            if (pair.second != local) {
                if (utils::isInside(pair.second->pos.x, pair.second->pos.y, 3 * 32, pos2f.x, pos2f.y)) {
                    draw->AddLine(ImVec2(ScreenLocal.x, ScreenLocal.y), ImVec2(ScreenPlayerLine.x, ScreenPlayerLine.y), ESPColor, 5);
                    draw->AddRect(ImVec2(ScreenPlayer.x, ScreenPlayer.y), ImVec2(footcam.x, footcam.y), ESPColor, 5.5f, 15, 5);
                }
                else {
                    draw->AddLine(ImVec2(ScreenLocal.x, ScreenLocal.y), ImVec2(ScreenPlayerLine.x, ScreenPlayerLine.y), ESPColor, 5);
                    draw->AddRect(ImVec2(ScreenPlayer.x, ScreenPlayer.y), ImVec2(footcam.x, footcam.y), ESPColor, 5.5f, 15, 5);
                }
            }
            draw->AddText(ImVec2(PlayerNameScreen.x, PlayerNameScreen.y + 15), ESPColor, namechar);
        }
    }
}

void doSpamWater() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        static types::time times = std::chrono::system_clock::now();
        static types::time timer = std::chrono::system_clock::now();
        auto locals = sdk::GetGameLogic()->GetNetObjMgr();
        auto it = sdk::GetGameLogic()->GetNetObjMgr()->players.begin();
        std::advance(it, rand() % sdk::GetGameLogic()->GetNetObjMgr()->players.size());
        GameUpdatePacket packet{ 0 };
        auto pos = local->GetPos();
        int x = local->GetPos().x / 32;
        int y = local->GetPos().y / 32;
        packet.type = PACKET_STATE;
        std::vector<int> A;
        A = { 2242, 2244, 2246, 2248, 2250 };
        if (opt::cheat::crystalmode && !opt::cheat::randommodepacketstate) {
            packet.int_data = A.at(utils::random(0, 4));
        }
        if (opt::cheat::randommodepacketstate && !opt::cheat::crystalmode) {
            auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            packet.int_data = rand() % 13158;
        }
        if (!opt::cheat::randommodepacketstate && !opt::cheat::crystalmode)
        {
            packet.int_data = opt::cheat::watervalue;
        }
        static int poggers;
        packet.int_x = (it->second->pos.x / 32) + utils::random(-16, 16);
        packet.int_y = (it->second->pos.y / 32) + utils::random(-16, 16);
        std::vector<int> flags;
        flags = { 32, 48 };
        if (utils::run_at_interval(timer, 0.5)) {
            poggers = flags.at(rand() % 2);
        }
        packet.flags = poggers | (1 << 10) | (1 << 11); //no enum rn so using raw values

        //Punch: 2560
        //Respawn: 2308   show

        packet.pos_x = pos.x;
        packet.pos_y = pos.y;
        if (utils::run_at_interval(times, 0.026)) {
            SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
        }

    }
}
void doWeatheresp() {
    auto logic = sdk::GetGameLogic();
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto tilemap = logic->GetTileMap();
    auto w2s = logic->GetWorldRenderer()->camera;
    auto rend = sdk::GetGameLogic()->renderer;
    if (logic->GetWorld()->name == "HEAVEN") {
        auto draw = ImGui::GetBackgroundDrawList();
        auto LOCALSCREEN = w2s.WorldToScreen(local->GetPos() + (local->GetSize() / 2.f));
        static int goalx = 32;
        static int goaly = 37;
        static types::time timerlol = std::chrono::system_clock::now();
        for (auto& tile : tilemap->tiles) {
            CL_Vec2f screenfirst;
            CL_Vec2f screensecond;
            screenfirst.x = tile.position_x * 32;
            screenfirst.y = tile.position_y * 32;
            screensecond.x = tile.position_x * 32;
            screensecond.y = tile.position_y * 32;
            auto screen = w2s.WorldToScreen(screenfirst);
            auto screen2 = w2s.WorldToScreen(screensecond);
            if (tile.foreground == 932 or tile.foreground == 934 or tile.foreground == 946 or tile.foreground == 984
                or tile.foreground == 1210 or tile.foreground == 1364 or tile.foreground == 1490 or tile.foreground == 1750
                or tile.foreground == 2046 or tile.foreground == 1364 or tile.foreground == 1490 or tile.foreground == 1750
                or tile.foreground == 2046 or tile.foreground == 3694 or tile.foreground == 5112 or tile.foreground == 7644
                or tile.foreground == 2284 or tile.foreground == 4242 or tile.foreground == 5654 or tile.foreground == 8836
                or tile.foreground == 2744 or tile.foreground == 4486 or tile.foreground == 5716 or tile.foreground == 8896
                or tile.foreground == 3252 or tile.foreground == 4776 or tile.foreground == 6280 or tile.foreground == 10058
                or tile.foreground == 3446 or tile.foreground == 4892 or tile.foreground == 6854 or tile.foreground == 10286
                or tile.foreground == 3534 or tile.foreground == 5000 or tile.foreground == 7282 or tile.foreground == 11880
                or tile.foreground == 12054 or tile.foreground == 12408 or tile.foreground == 12056 or tile.foreground == 12844
                or tile.foreground == 13070 or tile.foreground == 3832 or tile.foreground == 13004)
            {
                variantlist_t varlst{ "OnParticleEffect" };
                varlst[1] = 202;
                varlst[2] = vector2_t{ (float)tile.position_x * 32 + 15,  (float)tile.position_y * 32 + 15 };
                varlst[3] = 0;
                varlst[4] = 0;
                CL_Vec2f LocalPlayer;
                LocalPlayer.x = local->GetPos().x + 10;
                LocalPlayer.y = local->GetPos().y - (local->GetSize().y / 2);
                auto ScreenLocal = rend->GetCamera()->WorldToScreen(LocalPlayer);
                draw->AddLine(ImVec2(ScreenLocal.x, ScreenLocal.y), ImVec2(screen2.x, screen2.y), ESPColor, 5);
                draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ESPColor, 5);
                if (opt::cheat::weatherpunch) {
                    if (utils::isInside(tile.position_x * 32, tile.position_y * 32, 3 * 32, local->pos.x, local->pos.y))
                    {
                        static types::time time = std::chrono::system_clock::now();
                        if (utils::run_at_interval(time, 0.155)) {
                            gt::SendPunch(tile.position_x, tile.position_y);
                            gt::send_varlist_self(varlst);
                        }
                    }
                }
                if (opt::cheat::weatherpf) {
                    static types::time time = std::chrono::system_clock::now();
                    if (utils::isInside(tile.position_x * 32, tile.position_y * 32, 3 * 32, local->pos.x, local->pos.y))
                    {

                    }
                    else if (utils::run_at_interval(time, 0.5))
                    {
                        gt::pathFindTo(tile.position_x, tile.position_y);
                    }
                }

            }
        }
    }

}
void doDestroy() {
    auto logic = sdk::GetGameLogic();
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto tilemap = logic->GetTileMap();
    for (auto& tile : tilemap->tiles) {
        if (opt::cheat::autodestroybid == 0 && opt::cheat::autodestroyfid == 0) {
            if (tile.foreground != 0 or tile.background != 0)
            {
                variantlist_t varlst{ "OnParticleEffect" };
                varlst[1] = 202;
                varlst[2] = vector2_t{ (float)tile.position_x * 32 + 15,  (float)tile.position_y * 32 + 15 };
                varlst[3] = 0;
                varlst[4] = 0;
                if (utils::isInside(tile.position_x * 32, tile.position_y * 32, 3 * 32, local->pos.x, local->pos.y))
                {
                    static types::time time = std::chrono::system_clock::now();
                    if (utils::run_at_interval(time, 0.155)) {
                        gt::SendPunch(tile.position_x, tile.position_y);
                        gt::send_varlist_self(varlst);
                    }
                }
            }
        }
        else {
            if (tile.foreground == opt::cheat::autodestroyfid or tile.background == opt::cheat::autodestroybid)
            {
                variantlist_t varlst{ "OnParticleEffect" };
                varlst[1] = 202;
                varlst[2] = vector2_t{ (float)tile.position_x * 32 + 15,  (float)tile.position_y * 32 + 15 };
                varlst[3] = 0;
                varlst[4] = 0;
                if (utils::isInside(tile.position_x * 32, tile.position_y * 32, 3 * 32, local->pos.x, local->pos.y))
                {

                    static types::time time = std::chrono::system_clock::now();
                    if (utils::run_at_interval(time, 0.155)) {
                        gt::SendPunch(tile.position_x, tile.position_y);
                        gt::send_varlist_self(varlst);
                    }
                }
            }
        }
    }
}
void doClear() {
    auto logic = sdk::GetGameLogic();
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto tilemap = logic->GetTileMap();
    static types::time time = std::chrono::system_clock::now();
    for (auto& tiles : tilemap->tiles) {
        Tile* tile = tilemap->GetTileSafe(tiles.position_x, tiles.position_y);
        if (tile->foreground != 0 && tile->foreground != 8 && 14 && tile->foreground != 6 && tile->foreground != opt::cheat::bl1 &&
            tile->foreground != opt::cheat::bl2 && tile->foreground != opt::cheat::bl3 && tile->foreground != opt::cheat::bl4 &&
            tile->foreground != opt::cheat::bl5 && tile->foreground != opt::cheat::bl6 && tile->foreground != opt::cheat::bl7
            && tile->foreground != opt::cheat::bl8 && tile->foreground != opt::cheat::bl9 && tile->position_x != 0
            or tile->background != 0 && tile->background != opt::cheat::bl1 && tile->background != opt::cheat::bl2
            && tile->background != opt::cheat::bl3 && tile->background != opt::cheat::bl4 &&
            tile->background != opt::cheat::bl5 && tile->background != opt::cheat::bl6 && tile->background != opt::cheat::bl7 && tile->background != opt::cheat::bl8
            && tile->background != opt::cheat::bl9 && tile->position_x != 0) {
            variantlist_t varlst{ "OnParticleEffect" };
            varlst[1] = 202;
            varlst[2] = vector2_t{ (float)tiles.position_x * 32 + 15, (float)tiles.position_y * 32 + 15 };
            varlst[3] = 0;
            varlst[4] = 0;
            variantlist_t varlst2{ "OnParticleEffect" };
            varlst2[1] = 88;
            varlst2[2] = vector2_t{ (float)tiles.position_x * 32 + 15, (float)tiles.position_y * 32 + 15 };
            varlst2[3] = 0;
            varlst2[4] = 0;
            if (utils::isInside(tiles.position_x * 32, tiles.position_y * 32, opt::cheat::punchrange * 32, local->pos.x, local->pos.y)) {
                if (utils::run_at_interval(time, 0.155)) {
                    gt::SendPunch(tiles.position_x, tiles.position_y);
                    gt::send_varlist_self(varlst);
                }
            }
            else {
                if (utils::run_at_interval(time, 0.25)) {
                    // 1block
                    if (tilemap->GetTileSafe(tiles.position_x - 1, tiles.position_y)->foreground == 0) {
                        gt::send_varlist_self(varlst2);
                        gt::pathFindTo(tiles.position_x - 1, tiles.position_y);
                    }
                    else if (tilemap->GetTileSafe(tiles.position_x, tiles.position_y - 1)->foreground == 0) {
                        gt::send_varlist_self(varlst2);
                        gt::pathFindTo(tiles.position_x, tiles.position_y - 1);
                    }
                    else if (tilemap->GetTileSafe(tiles.position_x, tiles.position_y + 1)->foreground == 0) {
                        gt::send_varlist_self(varlst2);
                        gt::pathFindTo(tiles.position_x, tiles.position_y + 1);
                    }
                    else if (tilemap->GetTileSafe(tiles.position_x + 1, tiles.position_y)->foreground == 0) {
                        gt::send_varlist_self(varlst2);
                        gt::pathFindTo(tiles.position_x + 1, tiles.position_y);
                    }
                    else if (tilemap->GetTileSafe(tiles.position_x - 1, tiles.position_y - 1)->foreground == 0) {
                        gt::send_varlist_self(varlst2);
                        gt::pathFindTo(tiles.position_x - 1, tiles.position_y - 1);
                    }
                    else if (tilemap->GetTileSafe(tiles.position_x + 1, tiles.position_y - 1)->foreground == 0) {
                        gt::send_varlist_self(varlst2);
                        gt::pathFindTo(tiles.position_x + 1, tiles.position_y - 1);
                    }
                    else if (tilemap->GetTileSafe(tiles.position_x - 1, tiles.position_y + 1)->foreground == 0) {
                        gt::send_varlist_self(varlst2);
                        gt::pathFindTo(tiles.position_x - 1, tiles.position_y + 1);
                    }
                    else if (tilemap->GetTileSafe(tiles.position_x + 1, tiles.position_y + 1)->foreground == 0) {
                        gt::send_varlist_self(varlst2);
                        gt::pathFindTo(tiles.position_x + 1, tiles.position_y + 1);
                    }
                    // 2block
                    else if (tilemap->GetTileSafe(tiles.position_x - 2, tiles.position_y)->foreground == 0) {
                        gt::send_varlist_self(varlst2);
                        gt::pathFindTo(tiles.position_x - 2, tiles.position_y);
                    }
                    else if (tilemap->GetTileSafe(tiles.position_x, tiles.position_y - 2)->foreground == 0) {
                        gt::send_varlist_self(varlst2);
                        gt::pathFindTo(tiles.position_x, tiles.position_y - 2);
                    }
                    else if (tilemap->GetTileSafe(tiles.position_x, tiles.position_y + 2)->foreground == 0) {
                        gt::send_varlist_self(varlst2);
                        gt::pathFindTo(tiles.position_x, tiles.position_y + 2);
                    }
                    else if (tilemap->GetTileSafe(tiles.position_x + 2, tiles.position_y)->foreground == 0) {
                        gt::send_varlist_self(varlst2);
                        gt::pathFindTo(tiles.position_x + 2, tiles.position_y);
                    }
                }
            }
            return;
        }
        else if (tile->foreground != 0 && tile->foreground != 8 && 14 && tile->foreground != 6 && tile->foreground != opt::cheat::bl1 &&
            tile->foreground != opt::cheat::bl2 && tile->foreground != opt::cheat::bl3 && tile->foreground != opt::cheat::bl4 &&
            tile->foreground != opt::cheat::bl5 && tile->foreground != opt::cheat::bl6 && tile->foreground != opt::cheat::bl7
            && tile->foreground != opt::cheat::bl8 && tile->foreground != opt::cheat::bl9 or tile->background != 0 && tile->background != opt::cheat::bl1 &&
            tile->background != opt::cheat::bl2 && tile->background != opt::cheat::bl3 && tile->background != opt::cheat::bl4 &&
            tile->background != opt::cheat::bl5 && tile->background != opt::cheat::bl6 && tile->background != opt::cheat::bl7 && tile->background != opt::cheat::bl8
            && tile->background != opt::cheat::bl9) {
            variantlist_t varlst{ "OnParticleEffect" };
            varlst[1] = 202;
            varlst[2] = vector2_t{ (float)tiles.position_x * 32 + 15, (float)tiles.position_y * 32 + 15 };
            varlst[3] = 0;
            varlst[4] = 0;
            variantlist_t varlst2{ "OnParticleEffect" };
            varlst2[1] = 88;
            varlst2[2] = vector2_t{ (float)tiles.position_x * 32 + 15, (float)tiles.position_y * 32 + 15 };
            varlst2[3] = 0;
            varlst2[4] = 0;
            if (utils::isInside(tiles.position_x * 32, tiles.position_y * 32, opt::cheat::punchrange * 32, local->pos.x, local->pos.y)) {
                if (utils::run_at_interval(time, 0.155)) {
                    gt::SendPunch(tiles.position_x, tiles.position_y);
                    gt::send_varlist_self(varlst);
                }
            }
            else if (utils::run_at_interval(time, 0.25)) {
                if (tilemap->GetTileSafe(tiles.position_x, tiles.position_y - 1)->foreground == 0) {

                    gt::send_varlist_self(varlst2);
                    gt::pathFindTo(tiles.position_x, tiles.position_y - 1);
                }
                else if (tilemap->GetTileSafe(tiles.position_x + 1, tiles.position_y)->foreground == 0) {

                    gt::send_varlist_self(varlst2);
                    gt::pathFindTo(tiles.position_x + 1, tiles.position_y);
                }
                else if (tilemap->GetTileSafe(tiles.position_x, tiles.position_y + 1)->foreground == 0) {

                    gt::send_varlist_self(varlst2);
                    gt::pathFindTo(tiles.position_x, tiles.position_y + 1);
                }
                else if (tilemap->GetTileSafe(tiles.position_x + 1, tiles.position_y - 1)->foreground == 0) {

                    gt::send_varlist_self(varlst2);
                    gt::pathFindTo(tiles.position_x + 1, tiles.position_y - 1);
                }
                else if (tilemap->GetTileSafe(tiles.position_x + 1, tiles.position_y + 1)->foreground == 0) {

                    gt::send_varlist_self(varlst2);
                    gt::pathFindTo(tiles.position_x + 1, tiles.position_y + 1);
                }
                else if (tilemap->GetTileSafe(tiles.position_x, tiles.position_y - 2)->foreground == 0) {

                    gt::send_varlist_self(varlst2);
                    gt::pathFindTo(tiles.position_x, tiles.position_y - 2);
                }
                else if (tilemap->GetTileSafe(tiles.position_x, tiles.position_y + 2)->foreground == 0) {
                    gt::send_varlist_self(varlst2);
                    gt::pathFindTo(tiles.position_x, tiles.position_y + 2);
                }
                else if (tilemap->GetTileSafe(tiles.position_x + 2, tiles.position_y)->foreground == 0) {
                    gt::send_varlist_self(varlst2);
                    gt::pathFindTo(tiles.position_x + 2, tiles.position_y);
                }
            }
            return;
        }

    }
}

void doCustomblockESP() {
    auto logic = sdk::GetGameLogic();
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto tilemap = logic->GetTileMap();
    auto w2s = logic->GetWorldRenderer()->camera;
    auto rend = sdk::GetGameLogic()->renderer;

    auto draw = ImGui::GetBackgroundDrawList();
    auto LOCALSCREEN = w2s.WorldToScreen(local->GetPos() + (local->GetSize() / 2.f));
    auto tilepos = local->GetPos() / 32;
    static int goalx = 32;
    static int goaly = 37;
    static types::time timerlol = std::chrono::system_clock::now();
    for (auto& tile : tilemap->tiles) {
        CL_Vec2f screenfirst;
        CL_Vec2f screensecond;
        screenfirst.x = tile.position_x * 32;
        screenfirst.y = tile.position_y * 32;
        screensecond.x = tile.position_x * 32;
        screensecond.y = tile.position_y * 32;
        auto screen = w2s.WorldToScreen(screenfirst);
        auto screen2 = w2s.WorldToScreen(screensecond);
        if (tile.foreground == opt::cheat::customblockespid or tile.background == opt::cheat::customblockespid)
        {
            CL_Vec2f LocalPlayer;
            LocalPlayer.x = local->GetPos().x + 10;
            LocalPlayer.y = local->GetPos().y - (local->GetSize().y / 2);
            auto ScreenLocal = rend->GetCamera()->WorldToScreen(LocalPlayer);
            draw->AddLine(ImVec2(ScreenLocal.x, ScreenLocal.y), ImVec2(screen2.x, screen2.y), ESPColor, 5);
            draw->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen2.x, screen2.y), ESPColor, 5);

        }
    }

}
void doCustomitemESP() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();

    auto draw = ImGui::GetBackgroundDrawList();

    auto locals = sdk::GetGameLogic()->GetNetObjMgr();
    auto logic = sdk::GetGameLogic();
    auto pos2f = local->GetPos();
    auto objmap = logic->GetObjectMap();
    auto tilemap = logic->GetTileMap();
    auto rend = sdk::GetGameLogic()->renderer;

    float tilex = pos2f.x / 32;
    float tiley = pos2f.y / 32;
    for (auto it = objmap->objects.begin(); it != objmap->objects.end(); ++it) {

        CL_Vec2f item;
        item.x = it->pos.x - 3;
        item.y = it->pos.y - 3;
        CL_Vec2f itemy;
        itemy.x = it->pos.x + 19;
        itemy.y = it->pos.y + 19;
        auto ItemXYScreen = rend->GetCamera()->WorldToScreen(item);
        auto ItemWHScreen = rend->GetCamera()->WorldToScreen(itemy);
        CL_Vec2f LocalPlayer;
        LocalPlayer.x = local->GetPos().x + 10;
        LocalPlayer.y = local->GetPos().y - (local->GetSize().y / 2);
        auto ScreenLocal = rend->GetCamera()->WorldToScreen(LocalPlayer);
        if (it->item_id == opt::cheat::customitemespid) {

            draw->AddLine(ImVec2(ScreenLocal.x, ScreenLocal.y), ImVec2(ItemXYScreen.x, ItemXYScreen.y), ESPColor, 5.0f);
            draw->AddRect(ImVec2(ItemXYScreen.x, ItemXYScreen.y), ImVec2(ItemWHScreen.x, ItemWHScreen.y), ESPColor, 5.0f);

        }
    }

}

void doNazi() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    for (auto netid : sdk::GetGameLogic()->GetNetObjMgr()->players) {
        {
            if (opt::cheat::GetNetids == netid.first)
            {

                static types::time times = std::chrono::system_clock::now();
                static types::time timer = std::chrono::system_clock::now();
                static int i = 0;
                if (utils::run_at_interval(times, 0.025)) {
                    i += 1;
                    if (i > 16)
                        i = 0;
                }
                auto pos = local->GetPos();
                static int x, y;
                auto netidposx = netid.second->pos.x + 10; auto netidposy = netid.second->pos.y + 15;
                x = netidposx / 32, y = netidposy / 32;
                if (i == 0) //UrPos
                    x += 0, y += 0;
                //-----------------
                if (i == 1) //Right
                    x += 1, y += 0;
                if (i == 2) //Right
                    x += 2, y += 0;
                if (i == 3) //Up
                    x += 2, y -= 1;
                //Right Done
                //------------------
                if (i == 4) //Up
                    x += 0, y -= 1;
                if (i == 5) //Up
                    x += 0, y -= 2;
                if (i == 6) //Left
                    x -= 1, y -= 2;
                //Up Done
                //-----------------
                if (i == 7) //Left
                    x -= 1, y += 0;
                if (i == 8) //Left
                    x -= 2, y += 0;
                if (i == 9) //Down
                    x -= 2, y += 1;
                //Down Done
                //-----------------
                if (i == 10) //down
                    x += 0, y += 1;
                if (i == 11) //down
                    x += 0, y += 2;
                if (i == 12) //Left
                    x += 1, y += 2;
                //ALL DONE
                //---------------------------------- CORNERS
                if (i == 13) //right up
                    x += 2, y -= 2;
                if (i == 14) //left up
                    x -= 2, y -= 2;
                if (i == 15) //Right down
                    x += 2, y += 2;
                if (i == 16) //Left
                    x -= 2, y += 2;


                GameUpdatePacket packet{ 0 };
                packet.type = PACKET_STATE;
                packet.int_data = 2034;
                packet.int_x = x;
                packet.int_y = y;
                packet.pos_x = pos.x;
                packet.pos_y = pos.y;
                packet.vec_x = pos.x;
                packet.vec_y = pos.y;
                packet.vec2_x = 0;
                packet.vec2_y = 0;
                packet.flags = 32 | (1 << 10) | (1 << 11);

                if (utils::run_at_interval(timer, 0.025)) {
                    SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                }

            }
        }
    }
}

void doBlinkcolor() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        static types::time times = std::chrono::system_clock::now();
        std::vector<uint32_t> Skinlist;
        Skinlist = { 1348237567, 1685231359, 2022356223 };
        if (utils::run_at_interval(times, 0.2)) {
            string color = "action|setSkin\ncolor|" + std::to_string(Skinlist.at(utils::random(0, 7)));
            SendPacketHook::Execute(2, color, sdk::GetPeer());
            local->skin_color = Skinlist.at(utils::random(0, 7));
        }
    }
}
void embraceTheDarkness()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(8.00f, 8.00f);
    style.FramePadding = ImVec2(5.00f, 2.00f);
    style.CellPadding = ImVec2(6.00f, 6.00f);
    style.ItemSpacing = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.IndentSpacing = 25;
    style.ScrollbarSize = 15;
    style.GrabMinSize = 10;
    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 1;
    style.TabBorderSize = 1;
    style.WindowRounding = 7;
    style.ChildRounding = 4;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ScrollbarRounding = 9;
    style.GrabRounding = 3;
    style.LogSliderDeadzone = 4;
    style.TabRounding = 4;
}

void LoadFont(ImGuiIO io) {
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/calibrib.ttf", 18);
}

void killaura() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();

    for (auto netid : sdk::GetGameLogic()->GetNetObjMgr()->players)
    {
        if (opt::cheat::GetNetids == netid.first)
        {
            static types::time times = std::chrono::system_clock::now();
            static types::time timer = std::chrono::system_clock::now();

            variantlist_t varlst{ "OnParticleEffect" };
            varlst[1] = 88;
            varlst[2] = vector2_t{ netid.second->pos.x + 10,  netid.second->pos.y + 15 };
            varlst[3] = 0;
            varlst[4] = 0;
            GameUpdatePacket Punch{ 0 };
            auto pos = local->GetPos();
            Punch.type = PACKET_STATE;
            std::vector<int> A;
            A = { 822, 2034 };
            Punch.int_data = A.at(utils::random(0, 1));
            static int poggers;
            auto  netidposx = netid.second->pos.x + 10;
            auto  netidposy = netid.second->pos.y + 15;
            Punch.int_x = (netidposx / 32);
            Punch.int_y = (netidposy / 32);
            std::vector<int> flags;
            flags = { 32, 48 };
            if (utils::run_at_interval(timer, 0.5)) {
                poggers = flags.at(rand() % 2);
            }
            Punch.flags = poggers | 128 | 8390688 | (1 << 5) | (1 << 10) | (1 << 11) | 32768 | 8 | 16 | 32768;
            Punch.pos_x = pos.x;
            Punch.pos_y = pos.y;
            Punch.vec_x = pos.x;
            Punch.vec_y = pos.y;
            Punch.vec2_x = 0;
            Punch.vec2_y = 0;
            if (utils::run_at_interval(times, 0.025)) {
                SendPacketRawHook::Execute(4, &Punch, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                gt::send_varlist_self(varlst);
            }
        }
    }
}
void spampunch() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto tilemap = sdk::GetGameLogic()->GetTileMap();

    static types::time times = std::chrono::system_clock::now();
    static types::time timer = std::chrono::system_clock::now();
    auto locals = sdk::GetGameLogic()->GetNetObjMgr();
    // for (auto it = objmap->objects.begin(); it != objmap->objects.end(); ++it);
    auto it = sdk::GetGameLogic()->GetNetObjMgr()->players.begin();
    std::advance(it, rand() % sdk::GetGameLogic()->GetNetObjMgr()->players.size());

    if (it->second != local && utils::isInside(it->second->pos.x, it->second->pos.y, 100 * 100, local->pos.x, local->pos.y)) {
        Tile* tile = tilemap->GetTileSafe(it->second->pos / 32);
        GameUpdatePacket packet{ 0 };
        auto pos = local->GetPos();
        int x = local->GetPos().x / 32;
        int y = local->GetPos().y / 32;
        packet.type = PACKET_STATE;
        packet.int_data = 0;
        auto  itposx = it->second->pos.x + 10;
        auto  itposy = it->second->pos.y + 15;
        packet.int_x = itposx / 32;
        packet.int_y = itposy / 32;
        static int poggers;
        std::vector<int> flags;
        flags = { 32, 48 };
        if (utils::run_at_interval(timer, 0.5)) {
            poggers = flags.at(rand() % 2);
        }
        packet.flags = poggers | 8390688 | (1 << 5) | (1 << 10) | (1 << 11); // 3104 fake place?
        packet.pos_x = pos.x;
        packet.pos_y = pos.y;

        variantlist_t varlst{ "OnParticleEffect" };
        varlst[1] = 88;
        varlst[2] = vector2_t{ it->second->pos.x + 10, it->second->pos.y + 15 };
        varlst[3] = 0;
        varlst[4] = 0;
        if (tile->foreground != 12 && tile->foreground != 6 && tile->foreground != 585 && tile->foreground != 5362 && it->first != 9999 && it->first != local->netid) {
            if (utils::run_at_interval(times, 0.025)) {
                SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                gt::send_varlist_self(varlst);
            }
        }
    }

}
void GetUid() {
    opt::cheat::GetNetids = gt::GetNetIDByUID(opt::cheat::GetUids);
}
void doFreeTilePunch() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();

    static types::time times = std::chrono::system_clock::now();
    static types::time timer = std::chrono::system_clock::now();

    auto posx = opt::cheat::TouchPos.x;
    auto posy = opt::cheat::TouchPos.y;

    variantlist_t varlst{ "OnParticleEffect" };
    varlst[1] = 88;
    varlst[2] = vector2_t{ posx * 32,  posy * 32 };
    varlst[3] = 0;
    varlst[4] = 0;
    GameUpdatePacket Punch{ 0 };
    auto pos = local->GetPos();
    Punch.type = PACKET_STATE;
    std::vector<int> A;
    A = { 822, 2034 };
    Punch.int_data = A.at(utils::random(0, 1));
    static int poggers;
    Punch.int_x = (posx);
    Punch.int_y = (posy);
    std::vector<int> flags;
    flags = { 32, 48 };
    if (utils::run_at_interval(timer, 0.5)) {
        poggers = flags.at(rand() % 2);
    }
    Punch.flags = poggers | 128 | 8390688 | (1 << 5) | (1 << 10) | (1 << 11) | 32768 | 8 | 16 | 32768;
    Punch.pos_x = pos.x;
    Punch.pos_y = pos.y;
    Punch.vec_x = pos.x;
    Punch.vec_y = pos.y;
    Punch.vec2_x = 0;
    Punch.vec2_y = 0;
    if (utils::run_at_interval(times, 0.025)) {
        SendPacketRawHook::Execute(4, &Punch, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
        gt::send_varlist_self(varlst);
    }
}
void doFreeTileSpam() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();

    static types::time times = std::chrono::system_clock::now();
    static types::time timer = std::chrono::system_clock::now();

    auto posx = opt::cheat::TouchPos.x;
    auto posy = opt::cheat::TouchPos.y;

    variantlist_t varlst{ "OnParticleEffect" };
    varlst[1] = 88;
    varlst[2] = vector2_t{ posx * 32,  posy * 32 };
    varlst[3] = 0;
    varlst[4] = 0;
    GameUpdatePacket Punch{ 0 };
    auto pos = local->GetPos();
    Punch.type = PACKET_STATE;
    std::vector<int> A;
    A = { 2242, 2244, 2246, 2248, 2250 };
    if (opt::cheat::crystalmode) {
        Punch.int_data = A.at(utils::random(0, 4));
    }
    else {
        Punch.int_data = opt::cheat::freetilespamid;
    }
    static int poggers;
    Punch.int_x = (posx);
    Punch.int_y = (posy);
    std::vector<int> flags;
    flags = { 32, 48 };
    if (utils::run_at_interval(timer, 0.5)) {
        poggers = flags.at(rand() % 2);
    }
    Punch.flags = poggers | (1 << 5) | (1 << 10) | (1 << 11);
    Punch.pos_x = pos.x;
    Punch.pos_y = pos.y;
    Punch.vec_x = pos.x;
    Punch.vec_y = pos.y;
    Punch.vec2_x = 0;
    Punch.vec2_y = 0;
    if (utils::run_at_interval(times, 0.025)) {
        SendPacketRawHook::Execute(4, &Punch, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
        gt::send_varlist_self(varlst);
    }
}
void doNetidSpam() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();

    for (auto netid : sdk::GetGameLogic()->GetNetObjMgr()->players)
    {
        if (opt::cheat::GetNetids == netid.first)
        {
            static types::time times = std::chrono::system_clock::now();
            static types::time timer = std::chrono::system_clock::now();

            variantlist_t varlst{ "OnParticleEffect" };
            varlst[1] = 88;
            varlst[2] = vector2_t{ netid.second->pos.x + 10,  netid.second->pos.y + 15 };
            varlst[3] = 0;
            varlst[4] = 0;
            GameUpdatePacket Punch{ 0 };
            auto pos = local->GetPos();
            Punch.type = PACKET_STATE;
            std::vector<int> A;
            A = { 2242, 2244, 2246, 2248, 2250 };
            if (opt::cheat::crystalmode) {
                Punch.int_data = A.at(utils::random(0, 4));
            }
            else {
                Punch.int_data = opt::cheat::spamnetiditem;
            }
            static int poggers;
            auto  netidposx = netid.second->pos.x + 10;
            auto  netidposy = netid.second->pos.y + 15;
            Punch.int_x = (netidposx / 32);
            Punch.int_y = (netidposy / 32);
            std::vector<int> flags;
            flags = { 32, 48 };
            if (utils::run_at_interval(timer, 0.5)) {
                poggers = flags.at(rand() % 2);
            }
            Punch.flags = poggers | (1 << 5) | (1 << 10) | (1 << 11);
            Punch.pos_x = pos.x;
            Punch.pos_y = pos.y;
            Punch.vec_x = pos.x;
            Punch.vec_y = pos.y;
            Punch.vec2_x = 0;
            Punch.vec2_y = 0;
            if (utils::run_at_interval(times, 0.025)) {
                SendPacketRawHook::Execute(4, &Punch, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                gt::send_varlist_self(varlst);
            }
        }
    }
}
void doSpamTrade() {
    for (auto netid : sdk::GetGameLogic()->GetNetObjMgr()->players) {
        {

            if (opt::cheat::spamtrade && opt::cheat::GetNetids == netid.first)
            {
                static types::time times = std::chrono::system_clock::now();
                if (utils::run_at_interval(times, 0.025)) {
                    std::string packet = "action|trade_started\nnetid|" + to_string(opt::cheat::GetNetids);
                    SendPacketHook::Execute(2, packet, sdk::GetPeer());
                }
            }
        }
    }
}

void doNetidEsp() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto draw = ImGui::GetBackgroundDrawList();
    auto pos2f = local->GetPos();
    auto rend = sdk::GetGameLogic()->renderer;
    for (auto netid : sdk::GetGameLogic()->GetNetObjMgr()->players) {
        {

            if (opt::cheat::GetNetids == netid.first)
            {
                CL_Vec2f player;
                player.x = netid.second->pos.x - 8;
                player.y = netid.second->pos.y - 8;
                auto ScreenPlayer = rend->GetCamera()->WorldToScreen(player);
                CL_Vec2f playerline;
                playerline.x = netid.second->pos.x + 10;
                playerline.y = netid.second->pos.y - 8;
                auto ScreenPlayerLine = rend->GetCamera()->WorldToScreen(playerline);
                CL_Vec2f playername;
                playername.x = netid.second->pos.x + 5;
                playername.y = netid.second->pos.y + 30;
                auto PlayerNameScreen = rend->GetCamera()->WorldToScreen(playername);
                CL_Vec2f LocalPlayer;
                LocalPlayer.x = local->GetPos().x + 10;
                LocalPlayer.y = local->GetPos().y - (local->GetSize().y / 2);
                auto ScreenLocal = rend->GetCamera()->WorldToScreen(LocalPlayer);
                CL_Vec2f foot;
                foot.x = netid.second->pos.x + netid.second->GetSize().x + 5;
                foot.y = netid.second->pos.y + netid.second->GetSize().y + 5;
                auto footcam = rend->GetCamera()->WorldToScreen(foot);

                draw->AddLine(ImVec2(ScreenLocal.x, ScreenLocal.y), ImVec2(ScreenPlayerLine.x, ScreenPlayerLine.y), ESPColor, 5);
                draw->AddRect(ImVec2(ScreenPlayer.x, ScreenPlayer.y), ImVec2(footcam.x, footcam.y), ESPColor, 5.5f, 15, 5);

            }
        }
    }
}
void doFollow() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    for (auto netid : sdk::GetGameLogic()->GetNetObjMgr()->players) {
        {

            if (opt::cheat::followplayer && opt::cheat::GetNetids == netid.first)
            {
                if (utils::isInside(netid.second->pos.x, netid.second->pos.y, 7 * 32, local->pos.x, local->pos.y)) {
                    static types::time times = std::chrono::system_clock::now();
                    if (utils::run_at_interval(times, 0.001))
                    {
                        local->SetPos(netid.second->pos.x, netid.second->pos.y);
                    }
                }
                else {
                    static types::time time = std::chrono::system_clock::now();
                    if (utils::run_at_interval(time, 1)) {
                        gt::pathFindTo(netid.second->pos.x / 32, netid.second->pos.y / 32);
                    }
                }
            }
        }
    }
}
void doPfaround() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto logic = sdk::GetGameLogic();
    auto tilemap = logic->GetTileMap();
    for (auto netid : sdk::GetGameLogic()->GetNetObjMgr()->players) {
        {

            if (opt::cheat::pfaround && opt::cheat::GetNetids == netid.first)
            {
                static types::time time = std::chrono::system_clock::now();
                int x = utils::random(netid.second->pos.x / 32 - 3, netid.second->pos.x / 32 + 3);
                int y = utils::random(netid.second->pos.y / 32 - 3, netid.second->pos.y / 32 + 3);
                Tile* tile = tilemap->GetTileSafe(x, y);

                if (tile->foreground == 0 && tile->position_x >= -1 && tile->position_x <= 100 && tile->position_y >= -1 && tile->position_y <= 60) {
                    if (utils::run_at_interval(time, 0.035)) {
                        gt::pathFindTo(x, y);
                        return;
                    }

                }

            }
        }
    }
}
void doSelfpunch() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    int x = local->pos.x + 15;
    int y = local->pos.y + 15;
    static types::time times = std::chrono::system_clock::now();
    static types::time timer = std::chrono::system_clock::now();
    GameUpdatePacket packet{ 0 };
    auto pos = local->GetPos();
    packet.type = PACKET_STATE;
    packet.int_x = (x / 32);
    packet.int_y = (y / 32);
    packet.pos_x = pos.x;
    packet.pos_y = pos.y;
    packet.flags = 8390688;
    if (utils::run_at_interval(timer, 0.025)) {
        SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
    }
    if (utils::run_at_interval(times, 0.050)) {
        variantlist_t varlst{ "OnParticleEffect" };
        varlst[1] = 262;
        varlst[2] = vector2_t{ local->pos.x + 10, local->pos.y + 15 };
        varlst[3] = 0;
        varlst[4] = 0;
        gt::send_varlist_self(varlst);
    }
}
void doCarnivalgame() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    static types::time timer = std::chrono::system_clock::now();
    GameUpdatePacket door{ 0 };
    door.type = PACKET_TILE_ACTIVATE_REQUEST;
    door.int_data = 18;
    door.flags = 16;
    door.int_x = local->pos.x / 32;
    door.int_y = local->pos.y / 32;
    door.pos_x = local->pos.x;
    door.pos_y = local->pos.y;
    if (opt::cheat::carnivalgame == 0) {
        if (local->pos.x / 32 == 7 && local->pos.y / 32 == 39) {

        }
        else if (local->pos.x / 32 >= 6) {
            gt::pathFindTo(7, 37);
            if (utils::run_at_interval(timer, 0.350)) {
                SendPacketRawHook::Execute(4, &door, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                gt::sendlog("door");
            }
        }
        else if (local->pos.x / 32 <= 6) {
            gt::pathFindTo(3, 23);
            if (utils::run_at_interval(timer, 0.350)) {
                gt::SendPunch(local->pos.x / 32, local->pos.y / 32);
                gt::sendlog("end");
            }
        }
    }
    else if (opt::cheat::carnivalgame == 1) {
        if (local->pos.x / 32 == 92 && local->pos.y / 32 == 39) {

        }
        else if (local->pos.x / 32 <= 93) {
            gt::pathFindTo(92, 37);
            if (utils::run_at_interval(timer, 0.350)) {
                SendPacketRawHook::Execute(4, &door, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                gt::sendlog("door");
            }
        }
        else if (local->pos.x / 32 >= 93) {
            gt::pathFindTo(96, 23);
            if (utils::run_at_interval(timer, 0.350)) {
                gt::SendPunch(local->pos.x / 32, local->pos.y / 32);
                gt::sendlog("end");
            }
        }
    }
    else if (opt::cheat::carnivalgame == 2) {

    }
    else if (opt::cheat::carnivalgame == 3) {

    }
}
void ClickAllhRoulette()
{
    auto logic = sdk::GetGameLogic();
    auto world = logic->GetWorld();
    auto tilemap = logic->GetTileMap();



    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto mypos = local->GetPos();
    static types::time times = std::chrono::system_clock::now();

    if (world)
    {
        for (auto& tile : tilemap->tiles)
        {
            if (tile.foreground == 758)
            {
                auto UrPos = local->GetPos();
                GameUpdatePacket ht{ 0 };
                ht.type = PACKET_TILE_CHANGE_REQUEST;
                ht.int_data = 6326;
                ht.int_x = tile.position_x;
                ht.int_y = tile.position_y;
                ht.vec_x = UrPos.x;
                ht.vec_y = UrPos.y;
                if (utils::run_at_interval(times, 0.025)) {
                    gt::send(&ht);
                }
            }
        }
    }
}

void doSpamcactus() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        static types::time times = std::chrono::system_clock::now();
        static types::time timer = std::chrono::system_clock::now();
        static int poggers;
        std::vector<int> flags;
        flags = { 32, 48 };
        if (utils::run_at_interval(timer, 0.5)) {
            poggers = flags.at(rand() % 2);
        }

        auto pos = local->GetPos();
        static int x, y;
        x = pos.x / 32, y = pos.y / 32;

        GameUpdatePacket packet{ 0 };
        packet.type = PACKET_STATE;
        packet.int_data = 6336;
        packet.int_x = x;
        packet.int_y = y;
        packet.pos_x = pos.x;
        packet.pos_y = pos.y;
        //packet.flags = poggers | (1 << 10) | (1 << 11);
        packet.flags = poggers | 268435456;
        if (utils::run_at_interval(times, 0.025)) {
            SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
        }
    }
}
void doSpamjump() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local) {
        static types::time times = std::chrono::system_clock::now();
        static types::time timer = std::chrono::system_clock::now();
        static int poggers;
        std::vector<int> flags;
        flags = { 32, 48 };
        if (utils::run_at_interval(timer, 0.5)) {
            poggers = flags.at(rand() % 2);
        }

        auto pos = local->GetPos();
        static int x, y;
        x = pos.x / 32, y = pos.y / 32;

        GameUpdatePacket packet{ 0 };
        packet.type = PACKET_STATE;
        packet.int_data = 0;
        packet.int_x = x;
        packet.int_y = y;
        packet.pos_x = pos.x;
        packet.pos_y = pos.y;
        //packet.flags = poggers | (1 << 10) | (1 << 11);
        packet.flags = poggers | (1 << 10) | (1 << 11) | 128;
        if (utils::run_at_interval(times, 0.025)) {
            SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
        }
    }
}


void titlepos() {
    auto l = sdk::GetGameLogic()->GetLocalPlayer();
    int x = l->pos.x / 32;
    int y = l->pos.y / 32;
    variantlist_t varlist{ "OnNameChanged" };
    varlist[1] = l->name = "`^[" + std::to_string(x) + "," + std::to_string(y) + "]";
    gt::send_varlist_self(varlist, sdk::GetGameLogic()->GetLocalPlayer()->netid);
}


void Spammer() {
    static std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(100);
    const string wordList[8] = { "`2", "`3", "`4", "`#", "`9", "`8", "`c", "`6" };
    string word = wordList[rand() % 8];
    if (std::chrono::high_resolution_clock::now() - start_time >= std::chrono::seconds(opt::cheat::spaminterval))
    {
        start_time = std::chrono::high_resolution_clock::now();
        gt::send(2, "action|input\n|text|" + word + opt::cheat::spam_text);
    }
}
void Spammer2() {
    const string wordList[1] = { "|" };
    string word = wordList[rand() % 1];
    static std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(100);
    if (std::chrono::high_resolution_clock::now() - start_time >= std::chrono::seconds(opt::cheat::spaminterval))
    {
        start_time = std::chrono::high_resolution_clock::now();
        gt::send(2, "action|input\n|text" + word + opt::cheat::spam_text);
    }
}

void doNoname() {
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    if (local && opt::cheat::noname) {
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            pair.second->name = "`6me when``";
        }
    }

}


void loopRainbow() {
    switch (rmode) {
    case 1: {
        seco += 3;
        if (seco == 255) {
            rmode = 2;
        }
    } break;

    case 2: {
        firs -= 3;
        if (firs == 0) {
            rmode = 3;
        }
    } break;

    case 3: {
        thir += 3;
        if (thir == 255) {
            rmode = 4;
        }
    } break;

    case 4: {
        seco -= 3;
        if (seco == 0) {
            rmode = 5;
        }
    } break;

    case 5: {
        firs += 3;
        if (firs == 255) {
            rmode = 6;
        }
    } break;

    case 6: {
        thir -= 3;
        if (thir == 0) {
            rmode = 1;
        }
    } break;
    }
    ESPColor = ImColor(firs, seco, thir);
}
void UpdateMap() {
    auto logic = sdk::GetGameLogic();
    auto tilemap = logic->GetTileMap();
    if (logic) {
        for (auto& tile : tilemap->tiles) {
            if (tile.foreground != 0) {
                if (tile.foreground != 6 && tile.foreground != 410 && tile.foreground != 20 && tile.foreground != 26 && tile.foreground != 608 &&
                    tile.foreground != 780 && tile.foreground != 102 && tile.foreground != 1222 && tile.foreground != 4422 && tile.foreground != 2978 &&
                    tile.foreground != 3524 && tile.foreground != 762 && tile.foreground != 4632 && tile.foreground != 1308 && tile.foreground != 2862 &&
                    tile.foreground != 4798 && tile.foreground != 9740 && tile.foreground != 4240 && tile.foreground != 846 && tile.foreground != 5616 &&
                    tile.foreground != 9198 && tile.foreground != 1528 && tile.foreground != 3002 && tile.foreground != 430 && tile.foreground != 5036 &&
                    tile.foreground != 7444 && tile.foreground != 482 && tile.foreground != 12 && tile.foreground != 22 && tile.foreground != 16 &&
                    tile.foreground != 954 && tile.foreground != 3898 && tile.foreground != 1042 && tile.foreground != 6854 && tile.foreground != 8260 &&
                    tile.foreground != 5666 && tile.foreground != 340 && tile.foreground != 4722 && tile.foreground != 598 && tile.foreground != 2810 &&
                    tile.foreground != 554 && tile.foreground != 60 && tile.foreground != 1450 && tile.foreground != 8682 && tile.foreground != 5054 &&
                    tile.foreground != 4202 && tile.foreground != 3804 && tile.foreground != 4482 && tile.foreground != 1104 && tile.foreground != 4782 &&
                    tile.foreground != 3806 && tile.foreground != 2948 && tile.foreground != 8676 && tile.foreground != 858 && tile.foreground != 4740 &&
                    tile.foreground != 3796 && tile.foreground != 1700 && tile.foreground != 3232 && tile.foreground != 3572 && tile.foreground != 3566 &&
                    tile.foreground != 484 && tile.foreground != 5034 && tile.foreground != 546 && tile.foreground != 5032 && tile.foreground != 1446 &&
                    tile.foreground != 1604 && tile.foreground != 7164 && tile.foreground != 5040 && tile.foreground != 1684 && tile.foreground != 1702 &&
                    tile.foreground != 4704 && tile.foreground != 4706 && tile.foreground != 2072 && tile.foreground != 1162 && tile.foreground != 1240 &&
                    tile.foreground != 1770 && tile.foreground != 1420 && tile.foreground != 2586 && tile.foreground != 1422 && tile.foreground != 3522 &&
                    tile.foreground != 658 && tile.foreground != 428 && tile.foreground != 224 && tile.foreground != 4720 && tile.foreground != 998 &&
                    tile.foreground != 1326 && tile.foreground != 1752 && tile.foreground != 1324 && tile.foreground != 1682 && tile.foreground != 30 &&
                    tile.foreground != 4794 && tile.foreground != 4546 && tile.foreground != 3808 && tile.foreground != 10076 && tile.foreground != 10078 &&
                    tile.foreground != 2252 && tile.foreground != 2274 && tile.foreground != 62 && tile.foreground != 480 && tile.foreground != 24 &&
                    tile.foreground != 5660 && tile.foreground != 8020 && tile.foreground != 2244 && tile.foreground != 2242 && tile.foreground != 2246 &&
                    tile.foreground != 2248 && tile.foreground != 190 && tile.foreground != 192 && tile.foreground != 188 && tile.foreground != 758 &&
                    tile.foreground != 1256 && tile.foreground != 5468 && tile.foreground != 3072 && tile.foreground != 4352 && tile.foreground != 2272 &&
                    tile.foreground != 8994 && tile.foreground != 1482 && tile.foreground != 379 && tile.foreground != 10776 && tile.foreground != 9614 &&
                    tile.foreground != 886 && tile.foreground != 486 && tile.foreground != 1048 && tile.foreground != 3286 && tile.foreground != 3496 &&
                    tile.foreground != 3832 && tile.foreground != 756 && tile.foreground != 1436 && tile.foreground != 9268 && tile.foreground != 8634 &&
                    tile.foreground != 7362 && tile.foreground != 550 && tile.foreground != 286 && tile.foreground != 444 && tile.foreground != 2964 &&
                    tile.foreground != 1288 && tile.foreground != 854 && tile.foreground != 9330 && tile.foreground != 7984 && tile.foreground != 7888 &&
                    tile.foreground != 652 && tile.foreground != 1108 && tile.foreground != 1594 && tile.foreground != 398 && tile.foreground != 1434 &&
                    tile.foreground != 1304 && tile.foreground != 926 && tile.foreground != 656 && tile.foreground != 544 && tile.foreground != 1698 &&
                    tile.foreground != 974 && tile.foreground != 1694 && tile.foreground != 2646 && tile.foreground != 2068 && tile.foreground != 688 &&
                    tile.foreground != 1432 && tile.foreground != 9080 && tile.foreground != 686 && tile.foreground != 3126 && tile.foreground != 4698 &&
                    tile.foreground != 986 && tile.foreground != 4712 && tile.foreground != 5792 && tile.foreground != 6146 && tile.foreground != 5798 &&
                    tile.foreground != 3794 && tile.foreground != 6772 && tile.foreground != 1696 && tile.foreground != 4718 && tile.foreground != 596) {
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 596 && tile.flags == 32) { //Chest
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 596 && tile.flags == 0) { //Chest
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4712 && tile.flags == 64) { //Gorilla
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3126 && tile.flags == 1) { //Dark Magic Barrier
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3126 && tile.flags == 0) { //Dark Magic Barrier
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3126 && tile.flags == 32) { //Dark Magic Barrier
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 686 && tile.flags == 1) { //Jail Door
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 686 && tile.flags == 0) { //Jail Door
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 686 && tile.flags == 32) { //Jail Door
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3286 && tile.flags == 1024) { //Steam Door
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3286 && tile.flags == 1056) { //Steam Door
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3798 && tile.flags == 1) { //Vip Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3798 && tile.flags == 0) { //Vip Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 3798 && tile.flags == 32) { //Vip Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4352 && tile.flags == 0) { //Wolf Gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4352 && tile.flags == 32) { //Wolf Gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 224 && tile.flags == 0) { //House Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 224 && tile.flags == 32) { //House Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 224 && tile.flags == 48) { //House Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 224 && tile.flags == 24) { //House Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 224 && tile.flags == 288) { //House Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4240 && tile.flags == 0) { //Jade Portcullis
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4240 && tile.flags == 32) { //Jade Portcullis
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4240 && tile.flags == 24) { //Jade Portcullis
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 1162 && tile.flags == 0) { //Forcefield
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 1162 && tile.flags == 32) { //Forcefield
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 1162 && tile.flags == 24) { //Forcefield
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 4706 && tile.flags == 0) { //Adventure Barrier
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 7164 && tile.flags == 0) { //Red Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 7164 && tile.flags == 32) { //Red Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 7164 && tile.flags == 24) { //Red Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 7164 && tile.flags > 1000) { //Red Entrance
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5032 && tile.flags == 32) { //Dirt Gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5032 && tile.flags == 0) { //Dirt Gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5032 && tile.flags == 24) { //Dirt Gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5032 && tile.flags == 16) { //Dirt Gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 60 && tile.flags == 0) { //portuculis
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 60 && tile.flags == 32) { //portuculis
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 60 && tile.flags == 288) { //portuculis
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 2810 && tile.flags == 0) { //air vent
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 2810 && tile.flags == 32) { //air vent
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 2810 && tile.flags == 24) { //air vent
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 598 && tile.flags == 0) { //dragon gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 598 && tile.flags == 32) { //dragon gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 598 && tile.flags == 16) { //dragon gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 598 && tile.flags == 128) { //dragon gate
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5036 && tile.flags == 0) { //hidden door 3 VV
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5036 && tile.flags == 32) { //hidden door 3 VV
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5036 && tile.flags == 24) { //hidden door 3 VV
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5036 && tile.flags == 50) { //hidden door 3 VV
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }
                if (tile.foreground == 5036 && tile.flags > 1000) { //hidden door 3 VV
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 1;
                }

                if (tile.flags == 25) {
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.flags == 17) {
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.flags == 19) {
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.flags == 27) {
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 1042 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 6854 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 20 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 428 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 430 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 546 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 544 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                if (tile.foreground == 986 && tile.flags == 1) { //sumthing lol ahaha
                    opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
                }
                //986
            }
            if (tile.foreground == 0) {
                opt::cheat::gtmap[tile.position_y][tile.position_x] = 0;
            }
        }
    }
}

struct ExampleAppLog
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.

    ExampleAppLog()
    {
        AutoScroll = true;
        Clear();
    }

    void    Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void    Draw(const char* title, bool* p_open = NULL)
    {
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        Filter.Draw("Filter", -100.0f);

        ImGui::Separator();

        if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (clear)
                Clear();
            if (copy)
                ImGui::LogToClipboard();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* buf = Buf.begin();
            const char* buf_end = Buf.end();
            if (Filter.IsActive())
            {
                // In this example we don't use the clipper when Filter is enabled.
                // This is because we don't have random access to the result of our filter.
                // A real application processing logs with ten of thousands of entries may want to store the result of
                // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    if (Filter.PassFilter(line_start, line_end))
                        ImGui::TextUnformatted(line_start, line_end);
                }
            }
            else
            {
                // The simplest and easy way to display the entire buffer:
                //   ImGui::TextUnformatted(buf_begin, buf_end);
                // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
                // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
                // within the visible area.
                // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
                // on your side is recommended. Using ImGuiListClipper requires
                // - A) random access into your data
                // - B) items all being the  same height,
                // both of which we can handle since we have an array pointing to the beginning of each line of text.
                // When using the filter (in the block of code above) we don't have random access into the data to display
                // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
                // it possible (and would be recommended if you want to search through tens of thousands of entries).
                ImGuiListClipper clipper;
                clipper.Begin(LineOffsets.Size);
                while (clipper.Step())
                {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            ImGui::PopStyleVar();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();
        ImGui::End();
    }
};
enum heads {
    rage
};
enum sub_heads {
    cheats, players, world, misc, growscan, Misc, visual, realtime, player, networking,
};
static const ImWchar ranges[] =
{
    0x0020, 0x00FF, // Basic Latin + Latin Supplement
    0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
    0x2DE0, 0x2DFF, // Cyrillic Extended-A
    0xA640, 0xA69F, // Cyrillic Extended-B
    0xE000, 0xE226, // icons
    0,
};
ImFont* medium;
ImFont* bold;
ImFont* logo;
ImFont* tab_title;
ImFont* tab_title_icon;
ImFont* subtab_title;
ImFont* combo_arrow;
void menu::EndScene(IDirect3DDevice9* device, bool active) {
    if (!global::load) { //init imgui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplDX9_Init(device);
        embraceTheDarkness();
        LoadFont(io);
        global::load = true;
    }
    if (global::load) {
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        static bool showlogGUI = true;
        static bool showgui = false;
        static bool showgui2 = false;
        if (GetAsyncKeyState(VK_F2) & 1) {
            showgui = !showgui;
            opt::open = !opt::open;
        }
        if (GetAsyncKeyState(VK_F3) & 1)
            showgui2 = !showgui2;
        if (showgui2) {
            ImGui::ShowDemoWindow();
        }
        if (opt::console_open) {
            static ExampleAppLog log;

            // For the demo: add a debug button _BEFORE_ the normal log window contents
            // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
            // Most of the contents of the window will be added by the log.Draw() call.
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin("Example: Log");
            if (ImGui::SmallButton("[Debug] Add 5 entries"))
            {
                static int counter = 0;
                const char* categories[3] = { "info", "warn", "error" };
                const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
                for (int n = 0; n < 5; n++)
                {
                    const char* category = categories[counter % IM_ARRAYSIZE(categories)];
                    const char* word = words[counter % IM_ARRAYSIZE(words)];
                    log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                        ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
                    counter++;
                }
            }
            ImGui::End();

            // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
            log.Draw("Example: Log");
        }
        ImGui::End();
        ImGui::SetNextWindowSize({ 850, 730 });
        if (showgui && ImGui::Begin("##window", &showgui, ImGuiWindowFlags_NoDecoration)) {
            animate();
            static heads tab{ rage };
            static sub_heads subtab{ networking };
            ImFont* logo = nullptr;
            ImFont* tab_title = nullptr;
            ImFont* tab_title_icon = nullptr;
            auto draw = ImGui::GetWindowDrawList();

            auto pos = ImGui::GetWindowPos();
            auto size = ImGui::GetWindowSize();

            ImGuiStyle style = ImGui::GetStyle();
            
            // Menu
          


            const char* tab_name = tab == rage ? +"astralfarm" : 0;



            draw->AddText(tab_title, 23.0f, ImVec2(pos.x + 20, pos.y + 15), ImColor(255,255,255, 255), tab_name);

            switch (tab) {

            case rage:
                ImGui::SetCursorPos({ 10, 50 });
                ImGui::BeginGroup(); {
                    if (elements::subtab("Networking", subtab == networking)) { subtab = networking; }
                    if (elements::subtab("Real Time", subtab == realtime)) { subtab = realtime; }
                    if (elements::subtab("Players", subtab == player)) { subtab = player; }
                    if (elements::subtab("Automation", subtab == players)) { subtab = players; }
                    if (elements::subtab("Visual", subtab == visual)) { subtab = visual; }
                    if (elements::subtab("Cheats", subtab == cheats)) { subtab = cheats; }
                    if (elements::subtab("Item Database", subtab == world)) { subtab = world; }


                } ImGui::EndGroup();

                switch (subtab) {
                case networking:
                    ImGui::SetCursorPos({ 175, 16 });
                    ImGui::BeginChild("Networking"); {
                        menu::networking_tab();
                    }
                    ImGui::EndChild();
                }
                switch (subtab) {
                case realtime:
                    ImGui::SetCursorPos({ 175, 16 });
                    ImGui::BeginChild("Real Time"); {
                        menu::realtime_tab();
                    }
                    ImGui::EndChild();
                }
                switch (subtab) {
                case player:
                    ImGui::SetCursorPos({ 175, 16 });
                    ImGui::BeginChild("Players"); {
                        menu::player_tab();
                    }
                    ImGui::EndChild();
                }
                switch (subtab) {
                case cheats:
                    ImGui::SetCursorPos({ 175, 16 });
                    ImGui::BeginChild("Cheats"); {
                        menu::cheats_tab();
                    }
                    ImGui::EndChild();
                }
                switch (subtab) {
                case players:
                    ImGui::SetCursorPos({ 175, 16 });
                    ImGui::BeginChild("Automation"); {
                        menu::Automation();
                    }
                    ImGui::EndChild();
                }
                switch (subtab) {
                case world:
                    ImGui::SetCursorPos({ 175, 16 });
                    ImGui::BeginChild("Item Database"); {
                        menu::itemdata();
                    }
                    ImGui::EndChild();
                }
                switch (subtab) {
                case visual:
                    ImGui::SetCursorPos({ 175, 16 });
                    ImGui::BeginChild("Visual"); {
                        menu::visual_tab();
                    }
                    ImGui::EndChild();
                }
                switch (subtab) {
                case visual:
                    ImGui::SetCursorPos({ 175, 16 });
                    ImGui::BeginChild("Misc"); {
                        menu::visual_tab();
                    }
                    ImGui::EndChild();
                }
            }
            ImGui::End();
        
        }
        auto local = sdk::GetGameLogic()->GetLocalPlayer();
        if (local) {
            if (opt::cheat::modfly)
                doModfly();
            if (opt::visual::spamparticle)
                doSpamParticle();

            if (opt::cheat::playeresp)
                playeresp();
            if (opt::cheat::spamwater)
                doSpamWater();
            if (opt::cheat::autocollect)
                autocollect();
            if (opt::cheat::autofarm)
                doAutofarm();
            if (opt::cheat::blinkcolor)
                doBlinkcolor();
            if (opt::cheat::nazismoke)
                doNazi();
            if (opt::cheat::killaura)
                killaura();
            if (opt::cheat::punchaura)
                spampunch();
            if (opt::cheat::freetilepunch)
                doFreeTilePunch();
            if (opt::cheat::selfpunch)
                doSelfpunch();
            if (opt::cheat::spamtrade)
                doSpamTrade();
            if (opt::cheat::clickallroulette)
                ClickAllhRoulette();
            if (opt::cheat::titlepos)
                titlepos();
            if (opt::cheat::spammer && opt::cheat::coloredtext)
                Spammer();
            else if (opt::cheat::spammer)
                Spammer2();
            if (opt::cheat::followplayer)
                doFollow();
            if (opt::cheat::bruteforcepassdoor)
                doBruteforce();
            if (opt::cheat::noname)
                doNoname();
            if (opt::cheat::bubblespam)
                doBubblespam();
            if (opt::cheat::resspam)
                doResspam();
            if (opt::cheat::spamcactus)
                doSpamcactus();
            if (opt::cheat::jumpspam)
                doSpamjump();
            if (opt::cheat::netidnazi)
                doNazi();
            if (opt::cheat::pftoitem)
                doPftoitem();
            if (opt::cheat::weatheresp)
                doWeatheresp();
            if (opt::cheat::customblockesp)
                doCustomblockESP();
            if (opt::cheat::customitemesp)
                doCustomitemESP();
            if (opt::cheat::pfaround)
                doPfaround();
            if (opt::cheat::GetNetid)
                GetUid();
            if (opt::cheat::autodestroy)
                doDestroy();
            if (opt::cheat::autoclear)
                doClear();
            if (opt::cheat::autocarnival)
                doCarnivalgame();
            if (opt::cheat::freetilespam)
                doFreeTileSpam();
            if (opt::cheat::spamnetid)
                doNetidSpam();
            doNetidEsp();
        }
        loopRainbow();
        auto draw = ImGui::GetBackgroundDrawList();
        draw->AddText(NULL, 25.0f, ImVec2(10, 10), D3DCOLOR_XRGB(20,20,20), "astralfarm.xyz");
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    }
}

WNDPROC OriginalWndProcFunction;
bool menu::WndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    static bool init = false;
    if (!global::d9init)
        return false;

    if (global::load && !init) {
        ImGui_ImplWin32_Init(wnd);
        init = true;
    }
    //global::draw = true;
    if (msg == WM_KEYUP && wparam == VK_DELETE)
        global::draw = !global::draw;
    //= !global::draw;

    /*  if (msg == WM_KEYUP && wparam == VK_F4)
        load_config();*/
    ImGui_ImplWin32_WndProcHandler(wnd, msg, wparam, lparam);

    return CallWindowProcW(OriginalWndProcFunction, wnd, msg, wparam, lparam);
}

