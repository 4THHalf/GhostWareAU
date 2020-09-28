#pragma once

#include "pch-il2cpp.h"
#include "il2cpp-appdata.h"
#include <Windows.h>
#include <set>
#include <queue>
#include <optional>

using namespace app;

namespace Ghost {
	namespace State {
		extern bool ShowMenu;
		extern bool ShowRadar;
		extern bool MarkImpostors;

		extern std::optional<PlayerControl*> SelectedPlayer;
		extern std::optional<PlayerControl*> MurderTarget;
		extern std::optional<PlayerControl*> VoteTarget;
		extern std::optional<PlayerControl*> KickTarget;
	}
}
