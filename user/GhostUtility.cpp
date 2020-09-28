#include "GhostUtility.hpp"

std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10FFFF,
	std::codecvt_mode::little_endian>, char16_t> UTF16_TO_UTF8;

std::string GetUTF8StringFromNETString(app::String* netString)
{
	if (netString == NULL)
		return NULL;

	uint16_t* buffer = new uint16_t[netString->fields.m_stringLength + 1];
	memcpy(buffer, &netString->fields.m_firstChar, netString->fields.m_stringLength * sizeof(uint16_t));
	buffer[netString->fields.m_stringLength] = L'\0';
	std::string newString = UTF16_TO_UTF8.to_bytes((const char16_t*)buffer);

	delete[] buffer;

	return newString;
}

app::GameData_IHEKEPMDGIJ* GetPlayerData(app::PlayerControl* player) {
	return app::PlayerControl_get_Data(player, NULL);
}

std::vector<app::PlayerControl*> GetAllPlayers()
{
	std::vector<app::PlayerControl*> players = std::vector<app::PlayerControl*>();

	auto playerList = (*app::PlayerControl__TypeInfo).static_fields->AllPlayerControls;

	for (int i = 0; i < app::List_1_PlayerControl__get_Count(playerList, NULL); i++)
		players.push_back(app::List_1_PlayerControl__get_Item(playerList, i, NULL));

	return players;
}

app::GameData_IHEKEPMDGIJ* GetPlayerData(int8_t playerId) {
	for (auto player : GetAllPlayers()) {
		auto playerData = GetPlayerData(player);
		if (playerData->fields.FIOIBHIDDOC == playerId)
			return playerData;
	}
	return NULL;
}

int GetGameState()
{
	return (*app::AmongUsClient__TypeInfo).static_fields->Instance->fields._.GameState;
}

ImVec4 AmongUsColorToImVec4(app::Color color)
{
	return ImVec4(color.r, color.g, color.b, color.a);
}