#include "includes.hpp"
#include "offsets.hpp"
#include "memory_access.hpp"
#include "GameStatus.hpp"
#include "hook.hpp"
#include "dummyDevice.hpp"

LPDIRECT3DDEVICE9 pDevice = nullptr;
void* d3d9Device[119];
tEndScene oEndScene = nullptr;

GameStatus* game_status;

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR col) {
    D3DRECT rect = { x, y, x + w, y + h };

    pDevice->Clear(1, &rect, D3DCLEAR_TARGET, col, 0, 0);

}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color) {
    ID3DXLine* lineL;
    D3DXCreateLine(pDevice, &lineL);
    
    D3DXVECTOR2 line[2];
    line[0] = D3DXVECTOR2(x1, y1);
    line[1] = D3DXVECTOR2(x2, y2);
    lineL->SetWidth(thickness);
    lineL->Draw(line, 2, color);

    lineL->Release();
}

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
    if (!pDevice)
        pDevice = o_pDevice;

    

    for (int i = 1; i < 32; ++i) {
        Entity* entity = game_status->entity_list->entities[i].entity;
        if (!game_status->check_if_entity_valid(entity))
            continue;

        D3DCOLOR color;
        if (entity->team == game_status->local_entity->team)
            color = D3DCOLOR_ARGB(255, 0, 255, 0);
        else
            color = D3DCOLOR_ARGB(255, 255, 0, 0);


        Vec2 entity_on_screen;
        if (game_status->world_to_screen(entity->vec_origin, entity_on_screen)) {
            DrawLine(entity_on_screen.x, entity_on_screen.y, windowWidth / 2, windowHeight, 2, color);

            DrawLine(entity_on_screen.x-10, entity_on_screen.y, entity_on_screen.x+ 10, entity_on_screen.y, 2, color);
            DrawLine(entity_on_screen.x-10, entity_on_screen.y-20, entity_on_screen.x+ 10, entity_on_screen.y-20, 2, color);
            DrawLine(entity_on_screen.x-10, entity_on_screen.y, entity_on_screen.x- 10, entity_on_screen.y-20, 2, color);
            DrawLine(entity_on_screen.x+10, entity_on_screen.y, entity_on_screen.x+ 10, entity_on_screen.y-20, 2, color);
        }
    }

    oEndScene(pDevice);
}

DWORD WINAPI MainThread(HMODULE hModule)
{
    


    //HOOOOOOOOK
    if (getD3D9Device(d3d9Device, sizeof(d3d9Device))) {
        memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

        oEndScene = (tEndScene)trampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
    }
    game_status = new GameStatus;


    //Create console to help with debugging process (crucial infromation can be displayed there)
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    //Verify proper console allocation
    std::cout << "Hack injected!\n";

    std::cout << windowWidth << " : " << windowHeight << '\n';
    std::cout << "Source: " << std::hex << (uintptr_t)d3d9Device[42] << '\n';
    std::cout << "Destin: " << std::hex << (uintptr_t)hkEndScene << '\n';
    std::cout << "Gatewa: " << std::hex << (uintptr_t)oEndScene << '\n';

    while (true)
    {
        game_status->update();
        //End thread by clicking END key on the keyboard
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        //Read data from the game process memory
        int crosshair_id = game_status->local_entity->crosshair_id;//game_status.get_crosshair_id(game_status.get_local_player());
        int crosshair_team = game_status->entity_list->entities[crosshair_id - 1].entity->team;//game_status.get_team(game_status.get_player(crosshair_id - 1));
        int local_team = game_status->local_entity->team;//game_status.get_team(game_status.get_local_player());

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

    //UNHOOK
    patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

    //Close console and thread
    delete game_status;
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