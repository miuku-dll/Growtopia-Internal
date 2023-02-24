#pragma once
#include <core/globals.h>
#include <core/gt.h>
#include <proton\MiscUtils.h>
#include <sdk/sdk.h>
#include <proton/RTParam.hpp>
#include <menu/imgui/imgui.h>


static bool find_command(std::string chat, std::string name) {
    bool found = chat.find("/" + name) == 0;
    if (found)
        gt::sendlog("`6" + chat);
    return found;
}



class SendPacketHook {
public:
    static void Execute(int type, std::string& packet, ENetPeer* peer) {
        static auto orig = decltype(&hooks::SendPacket)(hookmgr->orig(sig::sendpacket));
        if (type > NET_MESSAGE_GAME_PACKET) //dont send any tracking packets, crash logs, or other shit to gt.
            return;


        if (packet.find("game_version|") != -1) {
            RTVar var = RTVar::parse(packet);
            opt::mac = gt::generate_mac();
            opt::rid = gt::generate_rid();
            auto hash_str = opt::mac + "RT";
            auto hash2 = HashString(hash_str.c_str(), hash_str.length());
            if (opt::spoof_login) {
                var.set("mac", opt::mac);
                var.set("wk", opt::rid);
                var.set("rid", opt::rid);
                var.set("fz", std::to_string(utils::random(INT_MIN, INT_MAX)));
                var.set("zf", std::to_string(utils::random(INT_MIN, INT_MAX)));
                var.set("hash", std::to_string(utils::random(INT_MIN, INT_MAX)));
                var.set("hash2", std::to_string(hash2));
                var.set("player_age", "20");
                gt::sendlog("`7Spoofing Info");
            }
            if (opt::spoof_name) {
                var.set("requestedName", utils::rnd(10));
            }
            if (opt::verspoof) {
                var.set("game_version", opt::gt_version);
                gt::sendlog("Spoofing Game Version");
            }
            if (opt::cheat::random_flag) {
                var.set("country", gt::get_random_flag());
            }
            else {
                var.set("country", opt::flag);
            }
            packet = var.serialize();
            gt::sendlog("`7Changing Sub Servers");
        }
        if (opt::cheat::GetNetid && !opt::cheat::GetUid)
        {
            if (packet.find("action|wrench") != -1) {
                opt::cheat::GetNetids = std::stoi(packet.substr(packet.find("netid|") + 6, packet.length() - packet.find("netid|") - 1));
                opt::cheat::GetUids = gt::GetUIDByNetid(opt::cheat::GetNetids);
            }
        }
        if (logging::enabled && logging::console & logging::sendpacket)
            printf("[SendPacket] type: %d\n%s\n", type, packet.c_str());
        printf("[SendPacket] type: %d\n%s\n", type, packet.c_str());

        string command = "action|input\n|text|/";
        if (packet.find(command + "sw ") != -1) {
            string world = packet.substr(command.size() + 3);
            gt::join_world(world);
            variantlist_t varlst{ "OnTextOverlay" };
            varlst[1] = "`7Warping to `5" + utils::toUpper(world);
            gt::send_varlist_self(varlst);
            return;
        }
        if (packet.find(command + "id ") != -1) {
            auto world = sdk::GetGameLogic()->GetWorld();
            string id = packet.substr(command.size() + 3);
            gt::join_world(world->name + "|" + id);
            variantlist_t varlst{ "OnTextOverlay" };
            varlst[1] = "`7Entering DoorID `5" + utils::toUpper(id);
            gt::send_varlist_self(varlst);
            return;
        }
        if (packet.find(command + "count ") != -1) {
            string count = packet.substr(command.size() + 6);
            opt::cheat::fastcount = count;
            if (count != "0") {
                variantlist_t varlst{ "OnTextOverlay" };
                varlst[1] = "`7Set Fast Drop/Trash Count To `b" + count;
                gt::send_varlist_self(varlst);
            }
            else if (count == "0") {
                variantlist_t varlst{ "OnTextOverlay" };
                varlst[1] = "`7Reset Fast Drop/Trash Count";
                gt::send_varlist_self(varlst);
            }
            return;
        }
        if (packet.find(command + "rndm") != -1) {
            gt::join_world(utils::rnd(16));
            variantlist_t varlst{ "OnTextOverlay" };
            varlst[1] = "`7Warping To A `bRandom `7World";
            gt::send_varlist_self(varlst);
            return;
        }
        if (packet.find(command + "wd ") != -1) {
            string countwl = packet.substr(command.size() + 3);
            gt::drop_item("242", countwl);
            return;
        }
        if (packet.find(command + "dd ") != -1) {
            string countdl = packet.substr(command.size() + 3);
            gt::drop_item("1796", countdl);
            return;
        }
        if (packet.find(command + "commands") != -1) {
            variantlist_t varlst{ "OnDialogRequest" };
            varlst[1] = "add_label_with_icon|big|`7Commands``|left|1458|"
                "\nadd_textbox|`c|left|"
                "\nadd_textbox|`c/sw [WORLD]  `7--  Free Warp|left|"
                "\nadd_textbox|`c/id [DOORID]  `7--  Enter DoorID|left|"
                "\nadd_textbox|`c/rndm  `7--  Warp To Random World|left|"
                "\nadd_textbox|`c/count  `7--  Fast Drop/Trash Count|left|"
                "\nadd_textbox|`c/wd  `7--  WL Drop|left|"
                "\nadd_textbox|`c/dd  `7--  DL Drop|left|"
                "\nadd_textbox|`c|left|"
                "\nadd_textbox|`cF5  `7--  Fast Trash/Drop Toggle|left|"
                "\nadd_textbox|`cF7  `7--  Clipboard Warp|left|"
                "\nadd_textbox|`cF8  `7--  SB Warp|left|"
                "\nadd_textbox|`cF9  `7--  Exit World|left|"
                "\nadd_textbox|`cF11  `7--  Go To Test World|left|"
                "\nend_dialog|Cancel|OK|";
            gt::send_varlist_self(varlst);
            return;
        }
        if (opt::visual::textcolorenable) {
            string text = "action|input\n|text|";
            if (packet.find(command) != -1) {
                string text1 = packet.substr(command.size());
                gt::send(2, "action|input\n|text|" + text1);
            }
            else if (packet.find(text) != -1) {
                string text2 = packet.substr(text.size());
                if (packet.find(text + opt::visual::textcolor) != -1) {

                }
                else {
                    gt::send(2, "action|input\n|text|" + opt::visual::textcolor + text2);
                    return;
                }
            }
        }
        orig(type, packet, peer);
    }
};