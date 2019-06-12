//-----------------------------------------------------------------------------
// @brief  三角形表示のチュートリアル.
// Copyright (c) 2016 Takeru Yui.
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include "Device.h"
#include "Game.h"

//-----------------------------------------------------------------------------
// グローバル変数群.
//-----------------------------------------------------------------------------
Device* g_device = NULL;
Game* g_game = NULL;

//-----------------------------------------------------------------------------
// @brief  グローバル変数群の解放.
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if (g_game != NULL)
    {
		g_game->Finalize();
		delete g_game;
		g_game = NULL;
	}
	if (g_device != NULL)
	{
		g_device->Finalize();
		delete g_device;
		g_device = NULL;
	}
}

//-----------------------------------------------------------------------------
// @brief  メッセージプロセス。ウインドウのメッセージを制御する.
// @note   Windowsプログラミングのための必須処理と思ってよい.
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // ウインドウの×ボタンが押されるなどしてウインドウが終了される場合、リソース解放を行う.
    switch (msg)
    {
    case WM_DESTROY:
        Cleanup();
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// @brief  アプリケーションのエントリーポイント
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
    // ウインドウ用のパラメータを設定.
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
        "D3D Tutorial", NULL };
    RegisterClassEx(&wc);

    // ウインドウを生成
    HWND hWnd = CreateWindow("D3D Tutorial", "D3D Tutorial 02: Vertices",
        WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
        NULL, NULL, wc.hInstance, NULL);

    // Direct3Dを初期化.
    g_device = new Device();
    if (g_device->Initialize(hWnd))
    {
		// ゲームクラス初期化.
		g_game = new Game();
		g_game->Initialize(*g_device);

        // ウインドウ表示.
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        // メッセージループ。WM_QUITが帰ってくるまでループを続ける.
        MSG msg;
        ZeroMemory(&msg, sizeof(msg));
        while (msg.message != WM_QUIT)
        {
            // 何かのメッセージがあればそれを処理する.
            if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
            {
                // 事前に設定したメッセージ処理用関数にメッセ―ジを処理させる。（この場合はMsgProc）.
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
				// 更新.
				g_game->Update();

                // 描画.
				g_game->Render(*g_device);
            }
        }
    }

    // ループを抜けたらウインドウを削除する.
    UnregisterClass("D3D Tutorial", wc.hInstance);
    return 0;
}
