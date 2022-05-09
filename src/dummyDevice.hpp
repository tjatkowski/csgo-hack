#pragma once

#include "includes.hpp"
#include "hook.hpp"

typedef HRESULT(APIENTRY* tEndScene) (LPDIRECT3DDEVICE9 pDevice);

extern int windowWidth, windowHeight;
extern HWND window;

extern BYTE EndSceneBytes[7];

BOOL CALLBACK enumWind(HWND handle, LPARAM lp);

HWND GetProcessWindow();

bool getD3D9Device(void** pTable, size_t size);









