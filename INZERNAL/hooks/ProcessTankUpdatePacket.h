#pragma once
#include <core/globals.h>
#include <core/gt.h>
#include <sdk/sdk.h>
#include <string>
#include <algorithm>
#include "core/discord_webhook.h"
#include <hooks/SendPacket.h>
#include <thread>
//#include <menu/menu.h>

/*
    {
    "content": "Webhooks are a low-effort way to post messages to channels in Discord. They do not require a bot user or authentication to use. https://discordapp.com/developers/docs/resources/webhook"
    }

*/
static int punch_effect = 8421376;


class ProcessTankUpdatePacketHook {
public:
    static void IsMod(int netID, std::string packet, std::string growid = "") {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        auto local = sdk::GetGameLogic()->GetLocalPlayer();
        auto players = sdk::GetGameLogic()->GetNetObjMgr()->players;
        auto tilemap = sdk::GetGameLogic()->GetTileMap();
        auto objmap = sdk::GetGameLogic()->GetObjectMap();
        auto pos2f = local->GetPos();

        for (auto g : players) {
            if (netID == g.second->netid || growid == g.second->name) {
                return;//Normal Player Joined to World
            }
        }
        variantlist_t asda{};
        asda[0] = "OnAddNotification";
        asda[1] = "interface/atomic_button.rttex";
        asda[2] = "`9Moderator Joined World";
        asda[3] = "audio/hub_open.wav";
        asda[4] = 0;
        gt::send_varlist_self(asda);
        gt::sendlog("Mod Name: " + growid);
        gt::sendlog("Mod NetID: " + to_string(netID));

            gt::sendlog("When Mod Join's Feature is on, Starting the Process!");
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (opt::cheat::modleave)
            {
                gt::send(3, "action|quit_to_exit", sdk::GetPeer());
            }
        
    }
    static void Execute(GameLogic* logic, GameUpdatePacket* packet) {
        static auto orig = decltype(&hooks::ProcessTankUpdatePacket)(hookmgr->orig(sig::processtankupdatepacket));
        ENetEvent* xd = nullptr;
        //just let the ghosts not even exist
        if (opt::cheat::antighost && packet->type == PACKET_NPC && (packet->npctype == 1 || packet->npctype == 7 || packet->npctype == 12))
            return;
        if (logging::packetview == false) { // View packets
            if (true)
                printf("===================================================\n");
            utils::printc("96", "[RECV]Raw Packet: %d [%s]\n\n", packet->type, gt::get_type_string(packet->type).c_str());
            variantlist_t varlist{};
            if (packet->type == PACKET_CALL_FUNCTION) {
                if (varlist.serialize_from_mem(utils::get_extended(packet))) {
                    auto content = varlist.print();
                    printf("Packet Info:\n%s\n", content.c_str());
                    printf("\n");
                }
            }
            else {
                printf("Packet Info: \n");
                if (packet->item != NULL) {
                    std::cout << "item: " << packet->item << std::endl;
                }
                if (packet->item_id != NULL) {
                    std::cout << "item_id: " << packet->item_id << std::endl;
                }
                if (packet->item_count != NULL && packet->item_count != 4294967295) {
                    std::cout << "item_count: " << packet->item_count << std::endl;
                }
                if (packet->gained_item_count != NULL) {
                    std::cout << "gained_item_count: " << packet->gained_item_count << std::endl;
                }
                if (packet->lost_item_count != NULL) {
                    std::cout << "lost_item_count: " << packet->lost_item_count << std::endl;
                }
                if (packet->objtype != NULL) {
                    std::cout << "objtype: " << packet->objtype << std::endl;
                }
                if (packet->punchid != NULL) {
                    std::cout << "punchid: " << packet->punchid << std::endl;
                }
                if (packet->pos_x != NULL) {
                    std::cout << "pos_x: " << packet->pos_x << std::endl;
                }
                if (packet->pos_y != NULL) {
                    std::cout << "pos_y: " << packet->pos_y << std::endl;
                }
                if (packet->int_x != NULL && packet->int_x != 4294967295) {
                    std::cout << "int_x: " << packet->int_x << std::endl;
                }
                if (packet->int_y != NULL && packet->int_y != 4294967295) {
                    std::cout << "int_y: " << packet->int_y << std::endl;
                }
                if (packet->build_range != NULL) {
                    std::cout << "build_range: " << packet->build_range << std::endl;
                }
                if (packet->punch_range != NULL) {
                    std::cout << "punch_range: " << packet->punch_range << std::endl;
                }
                if (packet->tiles_length != NULL) {
                    std::cout << "tiles_length: " << packet->tiles_length << std::endl;
                }
                if (packet->animation_type != NULL) {
                    std::cout << "animation_type: " << packet->animation_type << std::endl;
                }
                if (packet->flags != NULL) {
                    std::cout << "flags: " << packet->flags << std::endl;
                }
                printf("****************\nGameLogic:\n");
                if (logic->local != NULL) {
                    std::cout << "local: " << logic->local << std::endl;
                }
                if (logic->playeritems != NULL) {
                    std::cout << "playeritems: " << logic->playeritems << std::endl;
                }
                printf("\n");
                if (logic->world != NULL) {
                    std::cout << "world size: x: " << logic->GetTileMap()->size.x << " | y: " << logic->GetTileMap()->size.y << std::endl;
                    std::cout << "tile dmg: " << (int)logic->GetTileMap()->GetTileSafe(57, 14)->health_or_damage << std::endl;
                }
            }
        }
        auto local = sdk::GetGameLogic()->GetLocalPlayer();

        switch (packet->type) {
        case PACKET_CALL_FUNCTION: {
            if (logging::enabled && logging::console & logging::callfunction) {
                variantlist_t varlist{};
                auto extended = utils::get_extended(packet);
                if (varlist.serialize_from_mem(extended)) {
                    auto content = varlist.print();
                    if (content.length() < 4096)
                        printf("%s\n", content.c_str());
                }
            }
            bool test = true;
            if (test) {
                variantlist_t varlist{};
                auto extended = utils::get_extended(packet);
                if (varlist.serialize_from_mem(extended)) {
                    auto head = varlist.get(0);
                    if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("onShowCaptcha") != -1) {
                        auto menu = varlist[1].get_string();
                        auto g = utils::split(menu, "|");
                        if (opt::cheat::solve_captcha) {
                            std::string captchaid = g[1];
                            utils::replace(captchaid, "0098/captcha/generated/", "");
                            utils::replace(captchaid, "PuzzleWithMissingPiece.rttex", "");
                            captchaid = captchaid.substr(0, captchaid.size() - 1);

                            http::Request request{ "http://solar-bot.ga/captcha.php?captcha=" + captchaid };
                            const auto response = request.send("GET");
                            std::string output = std::string{ response.body.begin(), response.body.end() };
                            gt::send(2, "action|dialog_return\ndialog_name|puzzle_captcha_submit\ncaptcha_answer|" + output + "|CaptchaID|" + g[4]);
                            gt::sendlog("`7Solved capcha as `b" + output);
                            return;//success
                        }
                    }
                    if (head.get_string().find("ShowStartFTUEPopup") != -1) {
                        auto annoying = varlist.get(1).get_string();
                        if (annoying == "Welcome to Growtopia!") {
                            gt::send(2, "ftue_start_popup_close", sdk::GetPeer());
                            return;
                        }
                    }
                    /*if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnGuildDataChanged") != -1) {
                        variantlist_t varlst{ "OnAddNotification" };
                        varlst[1] = "interface/seth.rttex";
                        varlst[2] = "`#@MODRATOR `7IS IN THIS WORLD";
                        varlst[3] = "audio/already_used.wav";
                        varlst[4] = 0;

                        if (varlist[3].get_int32() == 5956) {
                            gt::send_varlist_self(varlst);
                            gt::sendlog("`#@Moderator`7 Entered The World");
                            if (opt::cheat::modleave) {
                                gt::send(3, "action|quit_to_exit");
                            }
                        }
                    }*/
                    if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnGuildDataChanged") != -1) {
                        variantlist_t varlst2{ "OnAddNotification" };
                        varlst2[1] = "interface/seth.rttex";
                        varlst2[2] = "`#Retard mod deteect";
                        varlst2[3] = "audio/already_used.wav";
                        varlst2[4] = 0;

                        if (varlist[3].get_int32() == 5956) {
                            gt::send_varlist_self(varlst2);
                            gt::sendlog("`#Kontol mod fp");
                            if (opt::cheat::modleave) {
                                gt::send(3, "action|quit_to_exit");
                            }
                        }
                        variantlist_t varlst{ "OnAddNotification" };
                        varlst[1] = "interface/seth.rttex";
                        varlst[2] = "`4GUARDIAN `7IS IN THIS WORLD";
                        varlst[3] = "audio/already_used.wav";
                        varlst[4] = 0;

                        if (varlist[3].get_int32() == 276) {
                            gt::send_varlist_self(varlst);
                            gt::sendlog("`4Guardian`7 Entered The World");
                        }
                    }
                    if (opt::cheat::antidetect) {
                        if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnConsoleMessage") != -1) {
                            auto console = varlist.get(1).get_string();
                            if (console.find("`6>>`4Spam detected!") != -1) {
                                opt::cheat::spammer = false;
                            }
                            static std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(100);
                            if (std::chrono::high_resolution_clock::now() - start_time >= std::chrono::milliseconds(20000))
                            {
                                start_time = std::chrono::high_resolution_clock::now();
                                opt::cheat::spammer = true;
                            }
                        }
                    }
                    if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnDialogRequest") != -1) {
                        auto content = varlist[1].get_string();
                        if (opt::cheat::autosurg && content.find("set_default_color|`o") != -1)
                        {
                            if (content.find("end_dialog|surgery||") != -1)
                            {
                                // maybe use switch statement is better hmmh
                                if (content.find("tool4312") != -1) { // defibrilator
                                    // use defibrilator
                                    gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4312");
                                }
                                else if (content.find("|tool1260|") == std::string::npos // scalpel
                                    && content.find("|tool1262|") == std::string::npos // anes
                                    && content.find("|tool1264|") == std::string::npos // antisept
                                    && content.find("|tool1266|") == std::string::npos // antibio
                                    && content.find("|tool1268|") == std::string::npos // splint
                                    && content.find("|tool1270|") == std::string::npos // stit
                                    && content.find("|tool4308|") == std::string::npos // pins
                                    && content.find("|tool4310|") == std::string::npos // transfusion
                                    && content.find("|tool4312|") == std::string::npos // defibrilator
                                    && content.find("|tool4314|") == std::string::npos // clamp
                                    && content.find("|tool4316|") == std::string::npos // ultrasound
                                    && content.find("|tool4318|") == std::string::npos // lab kit
                                    )
                                { // inefficient : D
                                    // use sponge
                                    gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1258");
                                }
                                else if (content.find("Patient is `6losing blood") != -1 || content.find("Patient is losing blood") != -1) {
                                    if (content.find("Fix It!") != -1) {
                                        // use stit
                                        gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                    }
                                    else if (content.find("tool4314") != -1) { // clamp
                                        // use clamp
                                        gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4314");
                                    }
                                    else {
                                        // use stit
                                        gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                    }
                                }
                                else if (content.find("Patient's fever is ") != -1) {
                                    if (content.find("tool4318") != -1) {
                                        // use lab kit
                                        gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                                    }
                                    else {
                                        // use antibio
                                        gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                    }
                                }
                                else if (content.find("Status: `6Coming to``|left|") != -1) {
                                    // use anes
                                    gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1262");
                                }
                                else if (content.find("add_smalltext|Pulse: `4Extremely Weak``") != -1) {
                                    // use transfusion
                                    gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4310");
                                }
                                else if (content.find(" shattered``") != -1 && content.substr(content.find("Incisions: `") + 13, 1) != "0") {
                                    // use pins
                                    gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4308");
                                }
                                else if (content.find(" broken``") != -1) {
                                    // use splint
                                    gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1268");
                                }
                                else if (content.find("Fix It!") != -1) {
                                    if (content.substr(content.find("Incisions: `") + 13, 1) != "0") {
                                        // use stit
                                        gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                                    }
                                    else {
                                        // use fix it
                                        gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1296"); // might be wrong
                                    }
                                }
                                else if (content.find("Temp: `6") != -1 || content.find("Temp: `4") != -1) {
                                    if (content.find("tool4318") != -1) {
                                        // use lab kit
                                        gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                                    }
                                    else {
                                        // use antibio
                                        gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                    }
                                }
                                else if (content.find("tool4316") == std::string::npos) { // ultrasound not found
                                    if (content.find("Status: `4Awake``|") == std::string::npos && content.find("Status: `3Awake``|") == std::string::npos) {
                                        // use scalpel
                                        gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1260");
                                    }
                                    else {
                                        // use anes
                                        gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1262");
                                    }
                                }
                                else if (content.find("tool4316") != -1) { // ultrasound found
                                    // use ultrasound
                                    gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4316");
                                }
                                else if (content.find("Temp: `2") != -1) {
                                    // use antibio
                                    gt::send(2, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                                }
                                return;
                            }
                        }
                        if (content.find("set_default_color|`o") != -1) {
                            if (content.find("add_label_with_icon|big|`wThe Growtopia Gazette``|left|5016|") != -1)
                            {
                                std::vector<string> A;
                                A = { "146.31.197.146", "179.50.125.40", "76.77.73.105", "94.167.184.15", "8.243.148.232", "34.131.243.175", "38.178.37.198", "112.230.30.137", "208.70.175.15", "180.142.57.10" };
                                variantlist_t varlst{ "OnAddNotification" };
                                varlst[1] = "";
                                varlst[2] = "`7" + A.at(utils::random(0, 9)) + "`4 Illumina On Top";
                                varlst[3] = "";
                                varlst[4] = 0;
                                gt::send_varlist_self(varlst);
                                return;
                            }
                            if (opt::cheat::GetNetid) {
                                std::string netidyarragi = content.substr(content.find("embed_data|netID") + 17, content.length() - content.find("embed_data|netID") - 1);
                                if (content.find("set_default_color|`o") != -1) {
                                    if (content.find("embed_data|netID") != -1) {
                                        return;
                                    }
                                }
                            }
                            int amountvend = 1;
                            if (opt::cheat::fastvend) {
                                if (content.find("How many would you like to buy?") != -1) {
                                    int x = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                                    int y = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                                    int itemid = std::stoi(content.substr(content.find("embed_data|expectitem|") + 22, content.length() - content.find("embed_data|expectitem|") - 1));
                                    int expectprice = std::stoi(content.substr(content.find("embed_data|expectprice|") + 23, content.length() - content.find("embed_data|expectprice|") - 1));
                                    int Lock = std::stoi(content.substr(content.find("add_textbox|You have") + 20, content.length() - content.find("add_textbox|You have") - 1));
                                    if (Lock - expectprice < 0) {
                                    }
                                    else {
                                        gt::send(2, "action|dialog_return\ndialog_name|vending\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nverify|1|\nbuycount|" + std::to_string(amountvend) + "|\nexpectprice|" + std::to_string(expectprice) + "|\nexpectitem|" + std::to_string(itemid) + "|");
                                        return;
                                    }
                                }
                            }
                            if (opt::cheat::emptybox) {
                                if (content.find("`4Retrieve All Gifts") != -1) {
                                    int x = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                                    int y = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));

                                    gt::send(2, "action|dialog_return\ndialog_name|donation_box_edit\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nbuttonClicked|clear");

                                    return;
                                }
                            }
                            if (opt::cheat::pullstock) {
                                if (content.find("Empty the machine") != -1) {
                                    int x = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                                    int y = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));

                                    gt::send(2, "action|dialog_return\ndialog_name|vending\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nbuttonClicked|pullstock");

                                    return;
                                }
                            }
                            if (opt::cheat::addstock) {
                                if (content.find("Update") != -1) {
                                    int x = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                                    int y = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));

                                    gt::send(2, "action|dialog_return\ndialog_name|vending\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nbuttonClicked|addstock");

                                    return;
                                }
                            }
                            if (opt::cheat::fastsign) {
                                if (content.find("would you like to write on this") != -1) {
                                    int x = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                                    int y = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));

                                    gt::send(2, "action|dialog_return\ndialog_name|sign_edit\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nsign_text|" + opt::cheat::fastsigntxt);

                                    return;
                                }
                            }
                            if (opt::cheat::fastboard) {
                                if (content.find("to conversation?") != -1) {
                                    int x = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                                    int y = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                                    gt::send(2, "action|dialog_return\ndialog_name|bulletin_edit\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "\nbuttonClicked|send\n\nsign_text|" + opt::cheat::fastboardtxt);

                                    return;
                                }
                            }
                            if (opt::cheat::fasttrash) {
                                std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
                                std::string count = content.substr(content.find("you have ") + 9, content.length() - content.find("you have ") - 1);
                                std::string s = count;
                                std::string delimiter = ")";
                                std::string token = s.substr(0, s.find(delimiter));
                                if (opt::cheat::fastcount == "0") {
                                    if (content.find("embed_data|itemID|") != -1) {
                                        if (content.find("Trash") != -1) {
                                            gt::send(2, "action|dialog_return\ndialog_name|trash_item\nitemID|" + itemid + "|\ncount|" + token);
                                            return;
                                        }
                                    }
                                }
                                else if (opt::cheat::fastcount != "0") {
                                    if (content.find("embed_data|itemID|") != -1) {
                                        if (content.find("Trash") != -1) {
                                            gt::send(2, "action|dialog_return\ndialog_name|trash_item\nitemID|" + itemid + "|\ncount|" + opt::cheat::fastcount);
                                            return;
                                        }
                                    }
                                }
                            }
                            if (opt::cheat::fastdrop) {
                                std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
                                std::string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
                                if (opt::cheat::fastcount == "0") {
                                    if (content.find("embed_data|itemID|") != -1) {
                                        if (content.find("Drop") != -1) {
                                            gt::send(2, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + count);
                                            return;
                                        }
                                    }
                                }
                                else if (opt::cheat::fastcount != "0") {
                                    if (content.find("embed_data|itemID|") != -1) {
                                        if (content.find("Drop") != -1) {
                                            gt::send(2, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + opt::cheat::fastcount);
                                            return;
                                        }
                                    }
                                }
                            }
                            if (opt::cheat::pathmarkerexploit) {
                                auto logic = sdk::GetGameLogic();
                                auto world = logic->GetWorld();
                                auto tilemap = logic->GetTileMap();
                                static types::time times = std::chrono::system_clock::now();
                                if (content.find("would you like to write on this") != -1 && opt::cheat::pathmarkerexploitlocalworld) {
                                    for (auto& tile : tilemap->tiles)
                                    {
                                        if (tile.foreground == 1684)
                                        {
                                            std::string packet = "action|dialog_return\ndialog_name|sign_edit\ntilex|" + to_string(tile.position_x) + "|\ntiley|" + to_string(tile.position_y) + "|\nsign_text|" + opt::cheat::pathmarkertxt;
                                            gt::send(2, packet);
                                            gt::join_world(world->name + "|" + opt::cheat::pathmarkertxt);
                                            return;
                                        }
                                    }
                                }
                                if (content.find("would you like to write on this") != -1 && opt::cheat::pathmarkerexploitotherworld) {
                                    static types::time times = std::chrono::system_clock::now();
                                    for (auto& tile : tilemap->tiles)
                                    {
                                        gt::join_world(opt::cheat::pathmarkerworld);
                                        if (tile.foreground == 1684)
                                        {
                                            if (utils::run_at_interval(times, 1)) {
                                                std::string packet = "action|dialog_return\ndialog_name|sign_edit\ntilex|" + to_string(tile.position_x) + "|\ntiley|" + to_string(tile.position_y) + "|\nsign_text|" + opt::cheat::pathmarkertxt;
                                                gt::send(2, packet);
                                                gt::join_world(world->name + "|" + opt::cheat::pathmarkertxt);
                                                return;
                                            }

                                        }
                                    }

                                }
                            }
                        }
                    }
                    if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnTextOverlay") != -1) {
                        auto overlay = varlist.get(1).get_string();

                        if (opt::cheat::auto_stop_farm) {
                            if (overlay.find("You were pulled by") != -1) {
                                opt::cheat::autofarm = false;
                                gt::send(2, "action|input\n|text|?");
                                opt::cheat::auto_stop_farm = false;

                            }
                        }
                    }
                    if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnSpawn") != -1) {
                        string rawPacket = varlist.get(1).get_string();
                        string playerName = utils::getValueFromPattern(rawPacket, "`6<`w(.*)``>``");
                        RTVar var = RTVar::parse(rawPacket);
                        auto name = var.find("name");
                        auto netID = var.find("netID");
                        auto userID = var.find("userID");
                        auto onlineID = var.find("onlineID");
                        auto isinvis = var.find("invis");
                        auto ismod = var.find("mstate");
                        auto isSmod = var.find("smstate");
                        if (name && netID && onlineID && isinvis && ismod && isSmod)
                        {

                            NetAvatar ply{};
                            bool isOwner = utils::toUpper(opt::cheat::owner_name) == utils::toUpper(utils::stripMessage(name->m_value));
                            if (opt::cheat::owner_detect && !isOwner && (name->m_value.find("`2") != -1 || name->m_value.find("`^") != -1)) {
                                opt::cheat::owner_netid.push_back(var.get_int("netID"));
                                gt::send(3, "action|quit_to_exit");
                                variantlist_t varlst{ "OnAddNotification" };
                                varlst[1] = "interface/seth.rttex";
                                varlst[2] = "`5 A World `2Owner`5/`^Admin`5 Has Been Detected, Leaving World...";
                                varlst[3] = "audio/already_used.wav";
                                varlst[4] = 0;
                                gt::send_varlist_self(varlst);

                            }
                            else if (!isOwner && opt::cheat::auto_ban) {
                                gt::send(2, "action|input\n|text|/ban " + utils::stripMessage(name->m_value));
                            }
                        }
                        if (opt::cheat::auto_pull) {
                            gt::send(2, "action|input\n|text|/pull " + utils::stripMessage(name->m_value));
                        }
                        if (opt::cheat::auto_msg) {
                            gt::send(2, "action|input\n|text|/msg " + name->m_values[0] + " " + opt::cheat::msg_text);
                        }
                        if (opt::cheat::start_msgall) {
                            if (opt::cheat::people_uid.size() <= opt::cheat::select_max) {
                                int uid = var.get_int("userID");
                                string name = var.get("name");
                                if (std::find(opt::cheat::people_uid.begin(), opt::cheat::people_uid.end(), uid) == opt::cheat::people_uid.end()) {
                                    opt::cheat::people_uid.push_back(uid);
                                    opt::cheat::people_name.push_back(name);
                                }
                            }
                        }
                    }
                    if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnTalkBubble") != -1) {
                        auto OnTalkBubble = varlist[2].get_string();
                        auto OnTalkBubble1 = varlist[1].get_string();

                        if (opt::cheat::spin_checker) {
                            if (OnTalkBubble.find("spun the wheel and got") != -1)
                            {
                                if (packet->int_data == 1800)
                                {
                                    varlist[2] = "`0[`2REAL```0]```` " + varlist[2].get_string();
                                    gt::send_varlist_self(varlist);
                                    return;
                                }
                            }
                        }
                    }
                    if (head.get_string().find("OnNameChanged") != -1) {
                        if (packet->effect_flags_check != sdk::GetGameLogic()->local->netid) {
                            std::thread ModTest(IsMod, packet->effect_flags_check, "OnNameChanged", varlist[1].get_string());
                            ModTest.detach();
                        }
                        else {
                            //gt::sendlog("Local OnNameChanged Event");
                        }
                    }
                    if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnConsoleMessage") != -1) {
                        auto content = varlist.get(1).get_string();
                        transform(content.begin(), content.end(), content.begin(), ::tolower);
                        if (opt::cheat::autogiveaway && content.find("`5** from") != -1 && content.find("giveaway") != -1) {
                            std::string packetC = "action|input\ntext|/go";
                            SendPacketHook::Execute(2, packetC, sdk::GetPeer());
                            opt::cheat::autogiveaway = false;
                        }
                        if (opt::cheat::autogiveaway && content.find("[bc]_") != -1 && content.find("giveaway") != -1) {
                            std::string packetC = "action|input\ntext|/go";
                            SendPacketHook::Execute(2, packetC, sdk::GetPeer());
                            opt::cheat::autogiveaway = false;
                        }
                        if (opt::cheat::autogiveaway && content.find("[cb]_") != -1 && content.find("giveaway") != -1) {
                            std::string packetC = "action|input\ntext|/go";
                            SendPacketHook::Execute(2, packetC, sdk::GetPeer());
                            opt::cheat::autogiveaway = false;
                        }

                    }


                }
            }
        }break;
        case PACKET_SEND_INVENTORY_STATE: {
            variantlist_t varlist{};
            auto extended = utils::get_extended(packet);
            break;
        }
        case PACKET_STATE: {

            break;
        }
        case PACKET_SET_CHARACTER_STATE: {
            if (!local)
                break;


            if (packet->netid == local->netid) {
                if (opt::cheat::antighost && global::state.gravity < packet->gravity_out && global::state.speed > packet->speed_out)
                    return;

                global::state.copy_from_packet(packet);
            }
        } break;
        }

        orig(logic, packet);
    }
};