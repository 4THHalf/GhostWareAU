#include "GhostState.hpp"

using namespace app;

namespace Ghost {
	namespace State {
		bool ShowMenu = false;
		bool ShowRadar = false;
		bool MarkImpostors = false;

		char Message[256] = "";

		std::optional<PlayerControl*> SelectedPlayer = std::nullopt;
		std::optional<PlayerControl*> MurderTarget = std::nullopt;
		std::optional<PlayerControl*> VoteTarget = std::nullopt;
		std::optional<PlayerControl*> KickTarget = std::nullopt;
	}
}