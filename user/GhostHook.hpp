#pragma once
#include "pch-il2cpp.h"
#include <Windows.h>
#include <d3d11.h>
#include <iostream>
#include "il2cpp-appdata.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "kiero.h"
#include "detours.h"
#include "GhostUtility.hpp"
#include "GhostState.hpp"
#include "GhostGUI.hpp"

typedef HRESULT(__stdcall* D3D_PRESENT_FUNCTION)(IDXGISwapChain*, UINT, UINT);

bool HookFunction(PVOID*, PVOID, const char*);
void HookInitilization();