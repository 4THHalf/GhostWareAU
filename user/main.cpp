#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "il2cpp-appdata.h"
#include "helpers.h"
#include "GhostHook.hpp"

using namespace app;

extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";

void Run()
{
    NewConsole();
    HookInitilization();
}