//-----------------------------------------------------------------------------
// @brief  三角形表示のチュートリアル.
// Copyright (c) 2016 Takeru Yui.
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include "Device.h"
#include "Camera.h"
#include "Triangle.h"

//-----------------------------------------------------------------------------
// グローバル変数群.
//-----------------------------------------------------------------------------
Device* g_device = NULL;
Triangle* g_triangle = NULL;
Camera* g_camera = NULL;

//-----------------------------------------------------------------------------
// @brief  グローバル変数群の解放.
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if (g_camera != NULL)
	{
		g_camera->Finalize();
		delete g_camera;
		g_camera = NULL;
	}
	if (g_triangle != NULL)
	{
		g_triangle->Finalize();
		delete g_triangle;
		g_triangle = NULL;
	}
    if (g_device != NULL)
    {
        g_device->Finalize();
        delete g_device;
        g_device = NULL;
    }
}

//-----------------------------------------------------------------------------
// @brief  シーンをレンダリングする.
//-----------------------------------------------------------------------------
VOID Render()
{
    // バックバッファを青色でクリアする.
    g_device->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

    // シーン描画を開始する.
    if (SUCCEEDED(g_device->GetDevice()->BeginScene()))
    {
		// カメラ設定.
		g_camera->SetCameraMatrices(*g_device);

        // マトリクス設定.
		g_triangle->SetWorldMatrices(*g_device);

		// レンダリング.
		g_triangle->Render(*g_device);

        // シーン描画を終了する.
        g_device->GetDevice()->EndScene();
    }

    // 実際の描画を確定する.
    g_device->GetDevice()->Present(NULL, NULL, NULL, NULL);
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
		g_triangle = new Triangle();
		g_triangle->Initialize(*g_device);

		g_camera = new Camera();
		g_camera->Initialize();

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
                // 描画.
                Render();
            }
        }
    }

    // ループを抜けたらウインドウを削除する.
    UnregisterClass("D3D Tutorial", wc.hInstance);
    return 0;
}
