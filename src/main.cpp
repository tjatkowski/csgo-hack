#include <Windows.h>
#include <iostream>

#define m_iTeamNum 0xF4
#define dwLocalPlayer 0xDB65DC
#define dwEntityList 0x4DD245C
#define m_iCrosshairId 0x11838

uintptr_t moduleBase;

template<typename T> T RPM(uintptr_t address) {
    try { return *(T*)address; }
    catch (...) { return T(); }
}

uintptr_t getLocalPlayer() { //This will get the address to localplayer. 
    return RPM< uintptr_t>(moduleBase + dwLocalPlayer);
}

uintptr_t getPlayer(int index) {  //Each player in the game has an index.
    return RPM< uintptr_t>(moduleBase + dwEntityList + index * 0x10); //We use index times 0x10 because the distance between each player 0x10.
}

int getTeam(uintptr_t player) {
    return RPM<int>(player + m_iTeamNum);
}

int getCrosshairID(uintptr_t player) {
    return RPM<int>(player + m_iCrosshairId);
}

DWORD WINAPI MainThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Welcome\n";

    moduleBase = (DWORD)GetModuleHandle("client.dll");


    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        int CrosshairID = getCrosshairID(getLocalPlayer());
        int CrosshairTeam = getTeam(getPlayer(CrosshairID - 1));
        int LocalTeam = getTeam(getLocalPlayer());

        if (CrosshairID > 0 && CrosshairID < 32 && LocalTeam != CrosshairTeam)
        {
            if (GetAsyncKeyState(VK_MENU /*alt key*/))
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
                Sleep(100); //Optional
            }
        }

    }
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}