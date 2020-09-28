#include "GhostGUI.hpp"

using namespace app;

namespace Ghost {
	namespace GUI {
		void RenderMenu(bool* state)
		{
			ImGui::Begin("GhostWare - Among Us", state);
			ImGui::SetWindowSize(ImVec2(400, 600));

			if (ImGui::CollapsingHeader("Game")) {
				ImGui::Checkbox("Mark Impostors", &Ghost::State::MarkImpostors);
			}

			if (ImGui::CollapsingHeader("Players") && (GetGameState() == 1 || GetGameState() == 2))
			{
				if (GetAllPlayers().size() > 0)
				{
					for (auto player : GetAllPlayers())
					{
						auto data = GetPlayerData(player);
						auto name = GetUTF8StringFromNETString(data->fields.EKHEPECKPKK);
						ImVec4 nameColor;

						if (data->fields.LODLBBJNGKB)
							nameColor = AmongUsColorToImVec4((*KMGFBENDNFO__TypeInfo).static_fields->MLKIANEMBLC);
						else
							nameColor = AmongUsColorToImVec4((*KMGFBENDNFO__TypeInfo).static_fields->OOJBHAIPPHP);

						if (data->fields.DMFDFKEJHLH)
							nameColor = AmongUsColorToImVec4((*KMGFBENDNFO__TypeInfo).static_fields->DMHEJEACLIG);

						bool selectionActive = (Ghost::State::SelectedPlayer.has_value() && player == Ghost::State::SelectedPlayer.value()) ? true : false;
						ImGui::PushStyleColor(ImGuiCol_Text, nameColor);
						if (ImGui::RadioButton(name.c_str(), selectionActive)) {
							if (Ghost::State::SelectedPlayer.has_value() && player == Ghost::State::SelectedPlayer.value())
								Ghost::State::SelectedPlayer = std::nullopt;
							else
								Ghost::State::SelectedPlayer = player;
						}
						ImGui::PopStyleColor(1);
					}

					/*if (ImGui::Button("Kick"))
						Ghost::State::KickTarget = Ghost::State::SelectedPlayer;

					ImGui::SameLine();*/

					std::string voteLabel;
					if (Ghost::State::VoteTarget.has_value()) {
						auto votePlayer = Ghost::State::VoteTarget.value();
						auto votePlayerData = GetPlayerData(votePlayer);
						auto votePlayerName = GetUTF8StringFromNETString(votePlayerData->fields.EKHEPECKPKK);
						voteLabel = "Vote Off " + votePlayerName;
					}
					else {
						voteLabel = "Vote Off";
					}

					if (ImGui::Button(voteLabel.c_str()))
						Ghost::State::VoteTarget = Ghost::State::SelectedPlayer;

					ImGui::SameLine();

					if (ImGui::Button("Teleport"))
					{
						if (Ghost::State::SelectedPlayer.has_value())
						{
							Transform* localTransform = Component_get_transform((Component*)(*PlayerControl__TypeInfo).static_fields->LocalPlayer, NULL);
							Transform* playerTransform = Component_get_transform((Component*)Ghost::State::SelectedPlayer.value(), NULL);
							Transform_set_position(localTransform, Transform_get_position(playerTransform, NULL), NULL);
						}
					}

					ImGui::SameLine();

					if (ImGui::Button("Murder"))
						Ghost::State::MurderTarget = Ghost::State::SelectedPlayer;
				}
			}

			ImGui::End();
		}
	}
}