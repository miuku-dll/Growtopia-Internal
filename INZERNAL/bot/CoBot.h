#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "CoreBot.h"
using namespace std;

namespace Mercury
{
	bool SelectAll = false;
	const int max_client = 20; // for now
	const char* clientNames[Mercury::max_client] = { "NULL" };
	std::vector<GrowtopiaBot*>bots;
	int current = -1;
	bool do_once_darildin_cak = false;
	void debug_log(string text) {
		std::cout << text << endl;
	}



	void addBot(string gid, string pwd, std::string gameversion, string world) {

		LoginInfo login;
		login.setGrowID(gid);



		login.password = pwd;
		login.macAddress = gt::generate_mac();
		login.version = gameversion;
		login.serverIp = "213.179.209.168";
		login.serverPort = 17199;
		bots.back()->startAsync();

	}
}