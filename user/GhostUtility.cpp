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

app::String* CreateNETStringFromANSI(const char* string)
{
	return app::Marshal_PtrToStringAnsi((void*)string, NULL);
}

app::String* CreateNETStringFromUTF8(const char* string)
{
	return app::Marshal_PtrToStringUni((void*)string, NULL);
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
	if ((*app::GameData__TypeInfo).static_fields->Instance != NULL) {
		return app::GameData_GetPlayerById((*app::GameData__TypeInfo).static_fields->Instance, playerId, NULL);
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