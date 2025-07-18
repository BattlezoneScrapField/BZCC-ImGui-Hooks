#include "DiscordManager.h"

#include <ScriptUtils.h>

#include <chrono>
#include <format>

DiscordManager::DiscordManager(const char* appID, const char* mode, const char* map)
	: appID(appID), sendPresence(true), startTime(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count())
{
	DiscordEventHandlers handlers{};

	gameMode = mode;
	gameMap = map;

	handlers.ready = HandleReady;
	handlers.disconnected = HandleDisconnected;
	handlers.errored = HandleErrored;
	handlers.joinGame = HandleJoin;
	handlers.spectateGame = HandleSpectate;
	handlers.joinRequest = HandleJoinRequest;

	Discord_Initialize(appID, &handlers, 1, nullptr);
}

DiscordManager::~DiscordManager()
{
	Discord_Shutdown();
}

void DiscordManager::Update()
{
	if (sendPresence)
	{
		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.state = gameMap;
		discordPresence.details = gameMode;
		discordPresence.startTimestamp = startTime;
		discordPresence.partySize = 1;
		discordPresence.partyMax = 4;
		Discord_UpdatePresence(&discordPresence);
	}
	else
	{
		Discord_ClearPresence();
	}
}

void DiscordManager::HandleReady(const DiscordUser* request)
{
	PrintConsoleMessage(std::format("Discord connected to user {}{} - {}", request->username, request->discriminator, request->userId).c_str());
}

void DiscordManager::HandleDisconnected(int errorCode, const char* message)
{
	PrintConsoleMessage("Disconnected");
}

void DiscordManager::HandleErrored(int errorCode, const char* message)
{
	PrintConsoleMessage("Error");
}

void DiscordManager::HandleJoin(const char* joinSecret)
{
	PrintConsoleMessage("Join");
}

void DiscordManager::HandleSpectate(const char* spectateSecret)
{
	PrintConsoleMessage("Spectate");
}

void DiscordManager::HandleJoinRequest(const DiscordUser* request)
{
	PrintConsoleMessage("Someone requested to join...");
}
