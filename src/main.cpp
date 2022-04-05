#include "includes.hpp"
#include "offsets.hpp"
#include "memory_access.hpp"
#include "GameStatus.hpp"

DWORD WINAPI MainThread(HMODULE hModule)
{
    //Create console to help with debugging process (crucial infromation can be displayed there)
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    //Verify proper console allocation
    std::cout << "Hack injected!\n";

    GameStatus game_status;


    while (true)
    {
        //End thread by clicking END key on the keyboard
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        //Read data from the game process memory
        int crosshair_id = game_status.get_crosshair_id(game_status.get_local_player());
        int crosshair_team = game_status.get_team(game_status.get_player(crosshair_id - 1));
        int local_team = game_status.get_team(game_status.get_local_player());

        if (crosshair_id > 0 && crosshair_id < 32 && local_team != crosshair_team)
        {
            if (GetAsyncKeyState(VK_MENU)) //Activate triggerbot while holding ALT key on the keyboard
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
                Sleep(100);
            }
        }

    }

    //Inform about the end of the thread
    std::cout << "Hack closed!\n";

    //Close console and thread
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

//dll main function. Creates new thread for our program
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
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