#include "GhostHook.hpp"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace app;

void (*oKeyboardJoystick_Update)(KeyboardJoystick*, MethodInfo*);
void (*oMeetingHud_Update)(MeetingHud*, MethodInfo*);
HatBehaviour__Array* (*oHatManager_GetUnlockedHats)(HatManager*, MethodInfo*);
PetBehaviour__Array* (*oHatManager_GetUnlockedPets)(HatManager*, MethodInfo*);
SkinData__Array* (*oHatManager_GetUnlockedSkins)(HatManager*, MethodInfo*);

ID3D11Device* pDevice;
ID3D11DeviceContext* pContext;
ID3D11RenderTargetView* pRenderTargetView;
D3D_PRESENT_FUNCTION oPresent;
HWND window;
WNDPROC oWndProc;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_KEYUP && wParam == VK_DELETE)
		Ghost::State::ShowMenu = !Ghost::State::ShowMenu;

	if (Ghost::State::ShowMenu || Ghost::State::ShowRadar) {
		if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(pDevice, pContext);

			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Ghost::State::ShowMenu) {
		Ghost::GUI::RenderMenu(&Ghost::State::ShowMenu);
	}

	ImGui::EndFrame();
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

void dKeyboardJoyststick_Update(KeyboardJoystick* __this, MethodInfo* method) {
	if (GetGameState() == 3) {
		Ghost::State::SelectedPlayer = std::nullopt;
		Ghost::State::MurderTarget = std::nullopt;
		Ghost::State::VoteTarget = std::nullopt;
		Ghost::State::KickTarget = std::nullopt;
	}

	/*ShipStatus* shipStatus = (*ShipStatus__TypeInfo).static_fields->Instance;
	if (shipStatus != NULL) {
		PlainShipRoom__Array* plainShipRooms = ShipStatus_get_AllRooms(shipStatus, NULL);
		if (plainShipRooms == NULL) {
			std::cout << "No Rooms" << std::endl;
		}
		else {
			std::cout << plainShipRooms->max_length << std::endl;
		}
	}*/

	/*if (GetGameState() == 1 || GetGameState() == 2) {
		if (Ghost::State::KickTarget.has_value()) {
			VoteBanSystem* vbSystem = (*VoteBanSystem__TypeInfo).static_fields->Instance;
			InnerNetClient* net = (InnerNetClient*)(*AmongUsClient__TypeInfo).static_fields->Instance;

			int32_t targetId = InnerNetClient_GetClientIdFromCharacter(net, (InnerNetObject*)Ghost::State::KickTarget.value(), NULL);
			int32_t selfId = net->fields.ClientId;

			for (auto player : GetAllPlayers()) {
				net->fields.ClientId = InnerNetClient_GetClientIdFromCharacter(net, (InnerNetObject*)player, NULL);
				VoteBanSystem_CmdAddVote(vbSystem, targetId, NULL);
			}

			net->fields.ClientId = selfId;
			Ghost::State::KickTarget = std::nullopt;
		}
	}*/

	if (GetGameState() == 2) {
		if (Ghost::State::MurderTarget.has_value()) {
			for (auto player : GetAllPlayers()) {
				if (GetPlayerData(player)->fields.LODLBBJNGKB && !GetPlayerData(player)->fields.DMFDFKEJHLH) {
					PlayerControl_RpcMurderPlayer(player, Ghost::State::MurderTarget.value(), NULL);
					break;
				}
			}
			Ghost::State::MurderTarget = std::nullopt;
		}
	}

	if (Ghost::State::MarkImpostors) {
		for (auto player : GetAllPlayers()) {
			auto playerData = GetPlayerData(player);
			TextRenderer* nameText = (TextRenderer*)player->fields.RemainingEmergencies;
			nameText->fields.Color = (playerData->fields.LODLBBJNGKB)
				? (*KMGFBENDNFO__TypeInfo).static_fields->MLKIANEMBLC
				: (*KMGFBENDNFO__TypeInfo).static_fields->OOJBHAIPPHP;
		}
	}

	oKeyboardJoystick_Update(__this, method);
}

void dMeetingHud_Update(MeetingHud* __this, MethodInfo* method) {
	MeetingHud_IONNOOOEADE__Enum state = (MeetingHud_IONNOOOEADE__Enum)((intptr_t)__this->fields.VoteEndingSound);

	if (Ghost::State::VoteTarget.has_value() && (state == 1 || state == 2))
	{
		auto targetData = GetPlayerData(Ghost::State::VoteTarget.value());
		auto targetName = GetUTF8StringFromNETString(targetData->fields.EKHEPECKPKK);

		for (auto player : GetAllPlayers()) {
			auto playerData = GetPlayerData(player);
			auto playerName = GetUTF8StringFromNETString(playerData->fields.EKHEPECKPKK);

			if (!MeetingHud_DidVote(__this, playerData->fields.FIOIBHIDDOC, NULL) && !playerData->fields.DMFDFKEJHLH) {
				MeetingHud_CmdCastVote(__this, playerData->fields.FIOIBHIDDOC, targetData->fields.FIOIBHIDDOC, NULL);
				break;
			}
		}
	}

	if (Ghost::State::VoteTarget.has_value() && state == 3 )
	{
		Ghost::State::VoteTarget = std::nullopt;
	}
	

	if (Ghost::State::MarkImpostors) {
		PlayerVoteArea__Array* playerStates = (PlayerVoteArea__Array*)(__this->fields.SkipVoteButton);
		for (int i = 0; i < playerStates->max_length; i++) {
			PlayerVoteArea* playerVoteArea = playerStates->vector[i];
			GameData_IHEKEPMDGIJ* playerData = GetPlayerData(playerVoteArea->fields.TargetPlayerId);
			if (playerData != NULL) {
				playerVoteArea->fields.NameText->fields.Color = (playerData->fields.LODLBBJNGKB)
					? (*KMGFBENDNFO__TypeInfo).static_fields->MLKIANEMBLC
					: (*KMGFBENDNFO__TypeInfo).static_fields->OOJBHAIPPHP;
			}
		}
	}

	oMeetingHud_Update(__this, method);
}

HatBehaviour__Array* dHatManager_GetUnlockedHats(HatManager* __this, MethodInfo* method) {
	return __this->fields.AllHats->fields._items;
}

PetBehaviour__Array* dHatManager_GetUnlockedPets(HatManager* __this, MethodInfo* method) {
	return __this->fields.AllPets->fields._items;
}

SkinData__Array* dHatManager_GetUnlockedSkins(HatManager* __this, MethodInfo* method) {
	return __this->fields.AllSkins->fields._items;
}

bool HookFunction(PVOID* ppPointer, PVOID pDetour, const char* functionName) {
	LONG err = DetourAttach(ppPointer, pDetour);
	if (err != 0) {
		std::cout << "HookFunction - " << functionName << " - Failed" << std::endl;
		return false;
	}
	else {
		std::cout << "HookFunction - " << functionName << " - Success" << std::endl;
		return true;
	}
}

void HookInitilization() {
	oKeyboardJoystick_Update = KeyboardJoystick_Update;
	oMeetingHud_Update = MeetingHud_Update;
	oHatManager_GetUnlockedHats = HatManager_GetUnlockedHats;
	oHatManager_GetUnlockedPets = HatManager_GetUnlockedPets;
	oHatManager_GetUnlockedSkins = HatManager_GetUnlockedSkins;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (!HookFunction(&(PVOID&)oKeyboardJoystick_Update, dKeyboardJoyststick_Update, "KeyboardJoystick_Update"))
		return;

	if (!HookFunction(&(PVOID&)oMeetingHud_Update, dMeetingHud_Update, "MeetingHud_Update"))
		return;

	if (!HookFunction(&(PVOID&)oHatManager_GetUnlockedHats, dHatManager_GetUnlockedHats, "HatManager_GetUnlockedHats"))
		return;

	if (!HookFunction(&(PVOID&)oHatManager_GetUnlockedPets, dHatManager_GetUnlockedPets, "HatManager_GetUnlockedPets"))
		return;

	if (!HookFunction(&(PVOID&)oHatManager_GetUnlockedSkins, dHatManager_GetUnlockedSkins, "HatManager_GetUnlockedSkins"))
		return;

	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		std::cout << "Kiero - Initialized" << std::endl;
		oPresent = (D3D_PRESENT_FUNCTION)kiero::getMethodsTable()[8];
		if (!HookFunction(&(PVOID&)oPresent, hkPresent, "D3D_PRESENT_FUNCTION"))
			return;
	}

	DetourTransactionCommit();
}