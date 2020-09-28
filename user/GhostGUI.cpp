#include "GhostGUI.hpp"

using namespace app;

namespace Ghost {
	namespace GUI {
		void RenderMenu(bool* state)
		{
			ImGui::Begin("GhostWare - Among Us", state);
			ImGui::SetWindowSize(ImVec2(400, 600), ImGuiCond_FirstUseEver);

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

					if (GetGameState() == 2) {
						std::string voteLabel;
						if (Ghost::State::VoteTarget.has_value()) {
							auto votePlayer = Ghost::State::VoteTarget.value();
							auto votePlayerData = GetPlayerData(votePlayer);
							auto votePlayerName = GetUTF8StringFromNETString(votePlayerData->fields.EKHEPECKPKK);
							voteLabel = "Next Vote - " + votePlayerName;
						}
						else {
							voteLabel = "Vote Off";
						}

						if (ImGui::Button(voteLabel.c_str())) {
							Ghost::State::VoteTarget = Ghost::State::SelectedPlayer;
						}

						ImGui::SameLine();
					}

					if (GetGameState() == 2) {
						if (ImGui::Button("Murder"))
							Ghost::State::MurderTarget = Ghost::State::SelectedPlayer;

						ImGui::SameLine();
					}

					if (GetGameState() == 2) {
						if (ImGui::Button("Call Meeting") && Ghost::State::SelectedPlayer.has_value())
							PlayerControl_CmdReportDeadBody(Ghost::State::SelectedPlayer.value(), NULL, NULL);

						ImGui::SameLine();
					}

					if (ImGui::Button("Teleport"))
					{
						if (Ghost::State::SelectedPlayer.has_value())
						{
							Transform* localTransform = Component_get_transform((Component*)(*PlayerControl__TypeInfo).static_fields->LocalPlayer, NULL);
							Transform* playerTransform = Component_get_transform((Component*)Ghost::State::SelectedPlayer.value(), NULL);
							Transform_set_position(localTransform, Transform_get_position(playerTransform, NULL), NULL);
						}
					}

					ImGui::Text("Message");
					ImGui::SameLine();
					if (ImGui::InputText("##Message", Ghost::State::Message, IM_ARRAYSIZE(Ghost::State::Message), ImGuiInputTextFlags_EnterReturnsTrue)) {
						if (Ghost::State::SelectedPlayer.has_value()) {
							PlayerControl_RpcSendChat(Ghost::State::SelectedPlayer.value(), CreateNETStringFromANSI(Ghost::State::Message), NULL);
							Ghost::State::Message[0] = '\0';
						}
					}
				}
			}

			if ((*PlayerControl__TypeInfo).static_fields->LocalPlayer != NULL
				&& GetPlayerData((*PlayerControl__TypeInfo).static_fields->LocalPlayer) != NULL
				&& GetPlayerData((*PlayerControl__TypeInfo).static_fields->LocalPlayer)->fields.IHACFCJPFCF != NULL
				&& ImGui::CollapsingHeader("Tasks")) {
				List_1_GameData_CBOMPDNBEIF_* tasks = GetPlayerData((*PlayerControl__TypeInfo).static_fields->LocalPlayer)->fields.IHACFCJPFCF;

				if (List_1_GameData_CBOMPDNBEIF__get_Count(tasks, NULL) > 0
					&& ImGui::Button("Complete All Tasks")) {
					for (int i = 0; i < List_1_GameData_CBOMPDNBEIF__get_Count(tasks, NULL); i++) {
						GameData_CBOMPDNBEIF* task = List_1_GameData_CBOMPDNBEIF__get_Item(tasks, i, NULL);

						if (!task->fields.MEAOHEIBBKN)
							PlayerControl_RpcCompleteTask((*PlayerControl__TypeInfo).static_fields->LocalPlayer, task->fields.AKLEDCMKHMC, NULL);
					}
				}

				ImGui::Spacing();

				for (int i = 0; i < List_1_GameData_CBOMPDNBEIF__get_Count(tasks, NULL); i++) {
					GameData_CBOMPDNBEIF* task = List_1_GameData_CBOMPDNBEIF__get_Item(tasks, i, NULL);

					if (ImGui::Button(("Complete##Button" + std::to_string(task->fields.AKLEDCMKHMC)).c_str()) && !task->fields.MEAOHEIBBKN)
						PlayerControl_RpcCompleteTask((*PlayerControl__TypeInfo).static_fields->LocalPlayer, task->fields.AKLEDCMKHMC, NULL);

					ImGui::SameLine();

					ImGui::TextColored(task->fields.MEAOHEIBBKN
						? ImVec4(0.0F, 1.0F, 0.0F, 1.0F)
						: AmongUsColorToImVec4((*KMGFBENDNFO__TypeInfo).static_fields->DMHEJEACLIG)
						, (std::string("Task #") + std::to_string(task->fields.GOBCMBALOFC)).c_str());
				}
			}
			ImGui::End();
		}
	}
}