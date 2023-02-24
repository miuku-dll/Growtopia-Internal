#pragma once
#include <core/globals.h>
#include <hooks/Update.h>
#include <sdk/sdk.h>
#include <string>
class SendPacketRawHook {
public:
    static void Execute(int type, GameUpdatePacket* packet, int size, void* packetsender, ENetPeer* peer, int flag) {
        static auto orig = decltype(&hooks::SendPacketRaw)(hookmgr->orig(sig::sendpacketraw));
        auto local = sdk::GetGameLogic()->GetLocalPlayer();
        if (opt::cheat::block_sendpacketraw)
            return;
        if (packet->type == PACKET_APP_INTEGRITY_FAIL) {
            utils::printc("95", "Was going to send a packet about app integrity failure, but we blocked it.");
            return;
        }
        if (packet->type == PACKET_STATE && opt::cheat::dancemove && !opt::cheat::spamwater && !opt::cheat::ghost) {
            packet->int_data = 0;
            packet->animation_type = 0;
            packet->flags = 16 | 8;
            packet->vec2_x = 0;
            packet->vec2_y = 0;
        }
        if (packet->type == PACKET_STATE && opt::cheat::ghost) {
            return;
        }
        if (packet->type == 11 && opt::cheat::anticollect) {
            return;
        }
        if (packet->type == PACKET_STATE) {
            GameUpdatePacket p{};
            p.type = PACKET_STATE;
            p.netid = 9999;
            p.int_data = packet->int_data;
            p.pos_x = packet->pos_x;
            p.pos_y = packet->pos_y;
            p.vec_x = packet->vec_x;
            p.vec_y = packet->vec_y;
            p.flags = packet->flags;
            p.int_x = packet->int_x;
            p.int_y = packet->int_y;
            gt::send_self(&p);
        }
        if (packet->type == PACKET_TILE_CHANGE_REQUEST) {
            if (packet->int_data == 18) {


            }
        }
        if (packet->type == PACKET_GOT_PUNCHED) {
            return;
        }
        if (packet->type == PACKET_SEND_PARTICLE_EFFECT) {
            printf("NetID: %d\n", (int)packet->netid);
            printf("PosX: %0f\n", (float)packet->vec_x);
            printf("PosY: %0f\n", (float)packet->vec_y);
            printf("XSpeed: %d\n", (int)packet->vec2_x);
            printf("YSpeed: %d\n", (int)packet->vec2_y);
            printf("MainValue: %d\n", (int)packet->int_data);

        }
        printf("sending raw packet: %d [%s]\n", packet->type, gt::get_type_string(packet->type).c_str());

        if (packet->type == 0 && packet->flags & 4) {
            auto player = sdk::GetGameLogic()->GetLocalPlayer();
            if (player)
                UpdateManager::OnJoinedWorld(player);
            for (auto otherplayerorme : sdk::GetGameLogic()->GetNetObjMgr()->players) {
                if (otherplayerorme.second != player && otherplayerorme.second->name != "illumina of Legend``") {
                    if (otherplayerorme.second->is_invis == true) {
                        otherplayerorme.second->is_invis = false;
                        gt::sendlog("`7Mod Name " + otherplayerorme.second->name);
                        if (opt::cheat::modleave) {
                            gt::send(3, "action|quit_to_exit");
                        }
                        variantlist_t varlst{ "OnAddNotification" };
                        varlst[1] = "interface/seth.rttex";
                        varlst[2] = "`#@Moderator`7 Entered The World";
                        varlst[3] = "audio/already_used.wav";
                        varlst[4] = 0;
                        gt::send_varlist_self(varlst);
                    }
                    otherplayerorme.second->is_invis = false;
                    otherplayerorme.second->is_supermod = false;
                    otherplayerorme.second->is_mod = false;
                    if (otherplayerorme.second->name == "````") {
                        gt::sendlog("`7Mod Name " + otherplayerorme.second->name);
                        if (opt::cheat::modleave) {
                            gt::send(3, "action|quit_to_exit");
                        }
                        variantlist_t varlst{ "OnAddNotification" };
                        varlst[1] = "interface/seth.rttex";
                        varlst[2] = "`#@Moderator`7 Entered The World";
                        varlst[3] = "audio/already_used.wav";
                        varlst[4] = 0;
                        gt::send_varlist_self(varlst);
                    }
                }
            }
        }

        if (packet->velocity_x == 1000.f || packet->type == PACKET_PING_REPLY) {
            GameUpdatePacket pk{ 0 };
            pk.type = PACKET_PING_REPLY;
            pk.ping_hash = packet->ping_hash; // HashString of ping request int data field
            pk.int_data = packet->int_data;   // tick count (speedhack)
            pk.pos_x = 64.f;                  // punch range in pixels
            pk.pos_y = 64.f;                  // build range in pixels
            pk.velocity_x = 1000.f;           //gravity
            pk.velocity_y = 250.f;            // movement speed

            void* PacketSender = nullptr;
            orig(NET_MESSAGE_GAME_PACKET, &pk, 56, PacketSender, peer, flag);
            return;
        }

        orig(type, packet, size, packetsender, peer, flag);
    }
};