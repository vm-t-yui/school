//-----------------------------------------------------------------------------
// @brief  Windowsのプロセスのラッパー.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------

#include "vmlib.h"

Device* g_device = NULL;
VMFbxManager* g_fbxManager = NULL;

WNDCLASSEX wc;
HWND hWnd;
MSG msg;

//-----------------------------------------------------------------------------
// @brief  DirectXデバイスを取得する.
//-----------------------------------------------------------------------------
LPDIRECT3DDEVICE9   GetDxDevice()
{
    assert(g_device != NULL);
    return g_device->GetDevice();
}

//-----------------------------------------------------------------------------
// @brief  FBXマネージャを取得する.
//-----------------------------------------------------------------------------
VMFbxManager* GetFbxManager()
{
    assert(g_fbxManager != NULL);
    return g_fbxManager;
}

//-----------------------------------------------------------------------------
// @brief  メッセージプロセス。ウインドウのメッセージを制御する.
// @note   Windowsプログラミングのための必須処理と思ってよい.
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // ウインドウの×ボタンが押されるなどするとウインドウが終了される.
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// @brief  ライブラリ初期化を行う.
//-----------------------------------------------------------------------------
int VMLib_Init()
{
    // ウインドウ用のパラメータを設定.
    wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
        "D3D Tutorial", NULL };
    RegisterClassEx(&wc);

    // ウインドウを生成
    hWnd = CreateWindow("D3D Tutorial", "D3D Tutorial 02: Vertices",
    WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
    NULL, NULL, wc.hInstance, NULL);

    // ウインドウ表示.
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
    ZeroMemory(&msg, sizeof(msg));

    // Direct3Dを初期化.
    g_device = new Device();
    if (!g_device->Initialize(hWnd))
    {
        return -1;
    }

    // FBXマネージャ作成.
    g_fbxManager = new VMFbxManager();
    g_fbxManager->Initialize();

    return 1;
}

//-----------------------------------------------------------------------------
// @brief  ライブラリ使用の後始末を行う.
//-----------------------------------------------------------------------------
int VMLib_End()
{
    // ウインドウを削除する.
    UnregisterClass("D3D Tutorial", wc.hInstance);

    if (g_device != NULL)
    {
        g_device->Finalize();
        delete g_device;
        g_device = NULL;
    }
    if (g_fbxManager != NULL)
    {
        g_fbxManager->Finalize();
        delete g_fbxManager;
        g_fbxManager = NULL;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// @brief  メッセージ処理関数.
//-----------------------------------------------------------------------------
int ProcessMessage()    
{
    // ウインドウズのメッセージループに代わる処理を行う
    // メッセージループ。WM_QUITが帰ってくるまでループを続ける.
    if (msg.message != WM_QUIT)
    {
        // 何かのメッセージがあればそれを処理する.
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            // 事前に設定したメッセージ処理用関数にメッセ―ジを処理させる。（この場合はMsgProc）.
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    else
    {
        return 0;
    }
    return 1;
}