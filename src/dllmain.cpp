// dllmain.cpp : Defines the entry point for the DLL application.

#include <lua.hpp>
#include <windows.h> // Note this is included BEFORE ScriptUtils cause otherwise it will overwrite some macros like RGB()
#include <ScriptUtils.h> // ScriptUtils.h isn't necessary for a pure lua dll but it's useful to have the native api at your disposal as well
#include "Hook.h"

#include <filesystem>
#include <format>

// As an aside, it's also possible to make a cross platform c++/lua dll that could serve as both a dll mission script as well as a lua library,
// you just need to declare the GetMisnApi properly as well as the lua library export function, they won't run at the same time of course
// but it's a cool idea. You could also make a c++ library that is usable in mission dlls as well as lua missions.

// Regular dllmain do as you please here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        PrintConsoleMessage("Running BZCCGui");

        DisableThreadLibraryCalls(hModule);
        Hook::hDDLModule = hModule;
        if (auto h = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Hook::HookDirectX, nullptr, 0, nullptr))
            CloseHandle(h);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        Hook::UnHookDirectX();
        break;
    }
    return TRUE;
}