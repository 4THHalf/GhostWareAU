#pragma once
#include "pch-il2cpp.h"
#include "il2cpp-appdata.h"
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <imgui.h>

std::string GetUTF8StringFromNETString(app::String* netString);
app::String* CreateNETStringFromANSI(const char* string);
app::String* CreateNETStringFromUTF8(const char* string);
app::GameData_IHEKEPMDGIJ* GetPlayerData(app::PlayerControl* player);
std::vector<app::PlayerControl*> GetAllPlayers();
app::GameData_IHEKEPMDGIJ* GetPlayerData(int8_t playerId);
int GetGameState();
ImVec4 AmongUsColorToImVec4(app::Color color);