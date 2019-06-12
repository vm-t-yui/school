//-----------------------------------------------------------------------------
// @brief  ウィンドウ表示のチュートリアル.
// Copyright (c) 2016 Takeru Yui.
//-----------------------------------------------------------------------------
#include <d3d9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

//-----------------------------------------------------------------------------
// グローバル変数群.
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL;          // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;    // Our rendering device

//-----------------------------------------------------------------------------
// @brief  Direct3Dの初期化.
//-----------------------------------------------------------------------------
bool InitD3D(HWND hWnd)
{
    // D3Dオブジェクトを生成.
    if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
        return false;

	// D3Dデバイス生成用の宣言.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    // D3Dデバイス生成
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &g_pd3dDevice) == false)
    {
        return false;
    }

	// 通常、デバイスの状態設定はここで行われる.
    // Device state would normally be set here

    return true;
}

//-----------------------------------------------------------------------------
// @brief  グローバル変数群の解放.
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}

//-----------------------------------------------------------------------------
// @brief  シーンをレンダリングする.
//-----------------------------------------------------------------------------
VOID Render()
{
	// バックバッファを青色でクリアする.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// シーン描画を開始する.
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// シーン描画を終了する.
		g_pd3dDevice->EndScene();
	}

	// 実際の描画を確定する.
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
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
	if (SUCCEEDED(InitD3D(hWnd)))
	{
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
