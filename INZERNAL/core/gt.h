#pragma once
#include <core/utils.h>
#include <proton/clanlib/vec2.h>
#include <string>
#include <unordered_map>


#include <Windows.h> //windows general header



namespace gt {
    //i mean tell me a simpler way to do this
    std::string generate_mac(const std::string& prefix = "02");
    std::string generate_rid();
    std::string GenerateWorld();
    std::string generate_meta();
    std::string get_random_flag();
    std::string get_type_string(uint8_t type);
    int16_t get_cpuid();
    int decrypt_piece(uint8_t* data, uint32_t size, int seed);
    int GetUIDByNetid(int NetID);
    int GetNetIDByUID(int UID);
    void decrypt_reg_vals();

    void set_extra_character_mods(NetAvatar* player, uint8_t flags);

    void send(int type, std::string message, bool hook_send = false);
    void send(GameUpdatePacket* packet, int extra_size = 0, bool hook_send = false);
    void send_self(GameUpdatePacket* packet, bool hook_send = true);
    void send_varlist_self(variantlist_t variantlist, int netid = -1, int delay = 0, bool hook_send = false);

    bool patch_banbypass();
    bool patch_mutex();
    void join_world(std::string world);

    //void screen_size(CL_Vec2f& rect);
    void pathFindTo(int x, int y);
    void hit_tile(CL_Vec2i where);
    void place_tile(int id, CL_Vec2i where);
    void wrench_tile(CL_Vec2i where);
    void water_tile(CL_Vec2i where);
    //void tutorial_bypass();
    //void calc_checksum(iteminfo_t& item);
    void sendlog(std::string text);


    void drop_item(std::string amount, std::string itemid);

    void WriteConfig(const char* filename);
    void AddBlack(std::string playername);

    int get_punch_id(const int id_);

    std::string lowercase(std::string s);

    void place_with_item_id(int x, int y, int itemid);

    void SendPunch(int x, int y);

    void ReadConfig(const char* filename);
    void itemdatas();



}

 

