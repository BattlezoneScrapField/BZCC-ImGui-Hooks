#pragma once

#include <discord_rpc.h>

#include <cstdint>

/*
* Some useful info from someone on a github issue lol -VT
* 
* To quote my documentation :
* When Start Time is included(not- null), the time in the rich presence will be shown as "00:01 elapsed".
* When End Time is included(not- null), the time in the rich presence will be shown as "00:01 remaining".If Start Time is set, this value will override the "elapsed" state to "remaining".
*/

class DiscordManager
{
public:
	DiscordManager(const char* appID, const char* mode, const char* map);
	~DiscordManager();

	void Update();

private:
	const char* appID;
	const char* gameMode;
	const char* gameMap;
	bool sendPresence;
	int64_t startTime;

	// These have to be static since they will be passed into a struct, they can't be
	// pointers to member functions
	static void HandleReady(const DiscordUser* request);
	static void HandleDisconnected(int errorCode, const char* message);
	static void HandleErrored(int errorCode, const char* message);
	static void HandleJoin(const char* joinSecret);
	static void HandleSpectate(const char* spectateSecret);
	static void HandleJoinRequest(const DiscordUser* request);
};