//    Grotopia Bot
//    Copyright (C) 2018  Growtopia Noobs
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "enet/include/enet.h"
#include <string>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <thread>
#include <core/gt.h>
#include <core/globals.h>
#include <sdk/sdk.h>


struct LoginInfo {
	void setGrowID(std::string gid) {
		growid = gt::lowercase(gid);
	}

	std::string getGrowID() {
		return growid;
	}

	int serverPort;
	std::string password;
	std::string macAddress;
	std::string version;
	std::string serverIp;

private:
	std::string growid;
};

struct gameupdatepacket_t {
	uint8_t m_type;
	uint8_t m_netid;
	uint8_t m_jump_amount;
	uint8_t m_count;
	int32_t m_player_flags;
	int32_t m_item;
	int32_t m_packet_flags;
	float m_struct_flags;
	int32_t m_int_data;
	float m_vec_x;
	float m_vec_y;
	float m_vec2_x;
	float m_vec2_y;
	float m_particle_time;
	uint32_t m_state1;
	uint32_t m_state2;
	uint32_t m_data_size;
	uint32_t m_data;
};




inline void SendString(ENetPeer* peer, ENetHost* host, int32_t type, std::string str) {
	if (str.size()) {
		ENetPacket* packet = enet_packet_create(0, str.size() + 5, ENET_PACKET_FLAG_RELIABLE);
		gametextpacket_t* game_packet = reinterpret_cast<gametextpacket_t*>(packet->data);
		game_packet->m_type = type;
		memcpy(&game_packet->m_data, str.data(), str.size());
		memset(&game_packet->m_data + str.size(), 0, 1);
		enet_peer_send(peer, 0, packet);
		enet_host_flush(host);
		//enet_packet_destroy(packet);
	}
}


using namespace std;

/*********  hidden vars  ********/
class GrowtopiaBot {
public:

	~GrowtopiaBot();


#pragma region CoreFuncs
	void start();
	void startAsync();
	void eventLoop();
	void hostService();
	bool connect(std::string address, int port);

#pragma region CoreEvents
	void onLoginRequested();
	void onPacketCallFunction(GameUpdatePacket* packet);
	virtual void onReady() = 0;
	virtual void onConnected() = 0;
	virtual void onDisconnected() = 0;
	virtual void onGameFunction(const std::string& func, variantlist_t& varlist) = 0;
#pragma endregion

#pragma region Funcs

#pragma endregion

	ENetHost* host = 0;
	ENetPeer* peer = 0;
	LoginInfo loginInfo;
#pragma endregion

	std::string follow;
	std::string spamtext;
	bool following = false;
	bool spamming = false;
	bool started = false;

	int  lastcollected = 0;
	std::string mega_txt;
	bool auto_reconnect = true;
	bool collecting = true;
	std::string fail_message = "";
	std::string uname = "wh1ter0sea";
	std::string upass = "whiterosesx1@";

	int count = 0;

	int subserver_port = 0, user = 0, token = 0;
	string UUIDToken = "";
	int num1 = 0;
	int num2 = 0;
	int startnum = 1;
	int startnum2 = 0;

	std::string version;
	std::string serverIp;
	int serverPort;
	std::atomic_bool running = true;
	std::thread poller_thread;
	std::string subserver_ip;


};

inline int8_t GetPacketType(ENetPacket* packet) {
	return (packet->dataLength > 3 ? *packet->data : 0);
}

inline std::string GetString(ENetPacket* packet) {
	std::string buffer;
	if (gametankpacket_t* tank = reinterpret_cast<gametankpacket_t*>(packet->data)) {
		memset(packet->data + packet->dataLength - 1, 0, 1);
		buffer.assign(static_cast<char*>(&tank->m_data));
	}
	return buffer;
	//return std::string(reinterpret_cast<char*>(packet->data + 4), packet->dataLength - 4);
}

inline gameupdatepacket_t* GetStruct(ENetPacket* packet) {
	if (packet->dataLength < sizeof(gameupdatepacket_t) - 4)
		return nullptr;
	gametankpacket_t* tank = reinterpret_cast<gametankpacket_t*>(packet->data);
	gameupdatepacket_t* gamepacket = reinterpret_cast<gameupdatepacket_t*>(packet->data + 4);
	if (gamepacket->m_packet_flags & 8) {
		if (packet->dataLength < gamepacket->m_data_size + 60) {
			printf("got invalid packet. (too small)\n");
			return nullptr;
		}
		return reinterpret_cast<gameupdatepacket_t*>(&tank->m_data);
	}
	else
		gamepacket->m_data_size = 0;
	return gamepacket;
}

