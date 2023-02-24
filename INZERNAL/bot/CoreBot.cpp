#include <sstream>
#include <vector>
#include <limits>
#include <math.h>
#include "CoreBot.h"
#include <core/HTTPRequest.hpp>
#include <proton/RTParam.hpp>
#include <core/httplib.h>


char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
'8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

std::string hexStr(unsigned char data)
{
	std::string s(2, ' ');
	s[0] = hexmap[(data & 0xF0) >> 4];
	s[1] = hexmap[data & 0x0F];
	return s;
}

string generateMeta()
{
	string x;
	for (int i = 0; i < 9; i++)
	{
		x += hexStr(rand());
	}
	x += ".com";
	return x;
}

string generateMac()
{
	string x;
	for (int i = 0; i < 6; i++)
	{
		x += hexStr(rand());
		if (i != 5)
			x += ":";
	}
	return x;
}

string generateRid()
{
	string x;
	for (int i = 0; i < 16; i++)
	{
		x += hexStr(rand());
	}
	for (auto& c : x) c = toupper(c);
	return x;
}


GrowtopiaBot::~GrowtopiaBot() {
	if (host && peer) {
		enet_peer_disconnect(peer, 0);
		enet_host_destroy(host);
	}
}

void GrowtopiaBot::start() {
	std::srand(std::time(nullptr));
	if (connect(serverIp, serverPort))
		eventLoop();
	else
		printf("failed to estabilish connection.\n");
}

void GrowtopiaBot::startAsync() {
	poller_thread = std::thread(&GrowtopiaBot::start, this);
}

void GrowtopiaBot::eventLoop() {
	try {
		while (running.load())
			hostService();
	}
	catch (...) {}
	running = false;
}

void GrowtopiaBot::hostService() {
	ENetEvent inEvent;
	while (enet_host_service(host, &inEvent, 50) > 0) {
		switch (inEvent.type) {
		case ENET_EVENT_TYPE_CONNECT:
			onConnected();
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			onDisconnected();
			return;
		case ENET_EVENT_TYPE_RECEIVE: {
			if (inEvent.packet) {
				int packet_type = GetPacketType(inEvent.packet);
				switch (packet_type) {
				case NET_MESSAGE_SERVER_HELLO: onLoginRequested(); break;
				case NET_MESSAGE_GAME_PACKET: {
					auto packet = GetStruct(inEvent.packet);
					if (!packet)
						break;
					switch (packet->m_type) {
					case PACKET_SET_ICON_STATE: break;
					case PACKET_ITEM_EFFECT: break;
					case PACKET_NPC: break;
					default: break;
					}
				} break;
				default: break;
				}
			}
			if (inEvent.packet)
				enet_packet_destroy(inEvent.packet);
			enet_host_flush(host);
		} break;
		default: break;
		}
	}
}

bool GrowtopiaBot::connect(std::string ipAddr, int port) {
	if (!ipAddr.empty() && port) {
		static bool do_once = true;
		if (do_once) {
			if (enet_initialize() < 0)
				printf("failed to init enet.\n");
			do_once = false;
		}
		if (host)
			enet_host_destroy(host);
		host = enet_host_create(0, 10, 0, 0, 0);
		if (host) {
			host->usingNewPacket = true;
			host->checksum = enet_crc32;
			enet_host_compress_with_range_coder(host);
			ENetAddress address;
			enet_address_set_host_ip(&address, ipAddr.c_str());
			address.port = port;
			peer = enet_host_connect(host, &address, 10, 0);
			if (peer) {
				printf("connected\n");
				enet_host_flush(host);
				return true;
			}
			else
				printf("failed to connect to the desired host.\n");
		}
		else
			printf("Failed to create enet host.\n");
	}
	return false;
}

void GrowtopiaBot::onLoginRequested() {
	try
	{
		http::Request request{ "http://a104-125-3-135.deploy.static.akamaitechnologies.com/growtopia/server_data.php" };
		const auto response = request.send("POST", "version=3.99&protocol=173&platform=0", { "Host: www.growtopia1.com" });
		RTVar var1 = RTVar::parse({ response.body.begin(), response.body.end() });

		auto HashStr = [&](std::string str) -> UINT { //refactored to perfection
			UINT buf = 0x55555555;
			for (BYTE& n : std::vector<BYTE>(str.begin(), str.end()))
				buf = (buf >> 27) + (buf << 5) + n;
			return buf;
		};
		std::mt19937 rng;
		std::uniform_int_distribution<int> distribution(INT_MIN, INT_MAX);
		string mac = "d0:37:45:69:48:2d";// for aap bypass
		string hash_str = mac + "RT";
		RTVar loginVar; // = rtvar::parse("tankIDName|mrec3o\ntankIDPass|11211453alp*\nrequestedName|SquishCake\nf|1\nprotocol|143\ngame_version|3.68\nfz|11058512\nlmode|0\ncbits|0\nplayer_age|16\nGDPR|1\nhash2|2087608419\nmeta|ni.com\nfhash|-716928004\nrid|018B57DF0BD79D2108669CA30434CC1E\nplatformID|0\ndeviceVersion|0\ncountry|us\nhash|-1815455562\nmac|02:8c:aa:20:e0:18\nwk|C2E5A6FCA9C9C2DBFC6EE67561E3BD35\nzf|-1807560130");
		//loginVar.set("tankIDName", loginInfo.getGrowID());
		//loginVar.set("tankIDPass", loginInfo.password);
		//loginVar.set("game_version", loginInfo.version);
		loginVar.append("tankIDName|" + loginInfo.getGrowID());
		loginVar.append("tankIDPass|" + loginInfo.password);
		loginVar.append("requestedName|SmileZero"); // :(
		loginVar.append("f|1");
		loginVar.append("protocol|175"); //sus
		loginVar.append("game_version|" + loginInfo.version);
		loginVar.append("fz|" + std::to_string(distribution(rng)));
		loginVar.append("lmode|0");
		loginVar.append("cbits|0");
		loginVar.append("player_age|26");
		loginVar.append("GDPR|1");
		loginVar.append("hash2|" + std::to_string(HashStr(hash_str))); //sus
		loginVar.append("meta|" + var1.get("meta"));
		loginVar.append("fhash|-716928004");
		loginVar.append("rid|" + utils::RandomStringA(32, "0123456789ABCDEF"));
		loginVar.append("gid|" + utils::RandomStringA(32, "0123456789ABCDEF"));
		loginVar.append("platformID|0");
		loginVar.append("deviceVersion|0");
		loginVar.append("country|us");
		loginVar.append("hash|" + std::to_string(distribution(rng)));
		loginVar.append("mac|" + string(mac));

		loginVar.set("mac", mac);
		loginVar.set("platformID", "4");
		loginVar.remove("fz");
		if (user && token) {
			loginVar.append("user|" + std::to_string(user));
			loginVar.append("token|" + std::to_string(token));
			if (token > -1)
			{
				loginVar.append("UUIDToken|" + UUIDToken);
				("UUIDToken: " + UUIDToken);
			}
			loginVar.set("lmode", "1");
		}
		//loginVar.append("wk|" + RandomString(32, "0123456789ABCDEF"));
		//loginVar.append("zf|" + std::to_string(distribution(rng)));
		gt::sendlog(loginVar.serialize());
		SendString(peer, host, 2, loginVar.serialize());
	}
	catch (const std::exception e)
	{
		std::cout << "Error Handled At Login Packet Sending dm srmotion" << e.what() << endl;
	}
}

void GrowtopiaBot::onPacketCallFunction(GameUpdatePacket* packet) {
	LPBYTE extended = utils::get_extended(packet);
	variantlist_t varlist{};
	std::string function(varlist.get(0).get_string());
	if (!function.empty()) {
		if (function == "OnSendToServer") {
			token = varlist.get(2).get_uint32() != -1 ? varlist.get(2).get_uint32() : token;
			user = varlist.get(3).get_uint32();
			string var4 = varlist.get(4).get_string();
			if (var4.find("|") != -1)
			{
				string values = utils::explode("|", var4)[2];
				if (values != "-1")

					UUIDToken = values;
				gt::sendlog("UUIDToken: " + UUIDToken);
			}
		}
		subserver_ip = varlist.get(4).get_string().erase(varlist.get(4).get_string().find("|"));
		gt::sendlog(subserver_ip);
		//varlist.get(4).get_string()
		//loginInfo.serverIp = varlist.get(4).get_string().erase(varlist.get(4).get_string().length() - 1);
		subserver_port = varlist.get(1).get_uint32();
		//loginInfo.serverPort = varlist.get(1).get_uint32();
		connect(subserver_ip, subserver_port);
	}
}