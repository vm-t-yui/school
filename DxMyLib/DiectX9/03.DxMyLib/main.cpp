//-----------------------------------------------------------------------------
// @brief  三角形表示のチュートリアル.
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
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;           // Buffer to hold vertices

//-----------------------------------------------------------------------------
// @brief 頂点描画に使用されるカスタム頂点構造体.
//-----------------------------------------------------------------------------
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

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
// @brief  VertexBuffuerの初期化.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name: InitVB()
// Desc: Creates a vertex buffer and fills it with our vertices. The vertex
//       buffer is basically just a chuck of memory that holds vertices. After
//       creating it, we must Lock()/Unlock() it to fill it. For indices, D3D
//       also uses index buffers. The special thing about vertex and index
//       buffers is that they can be created in device memory, allowing some
//       cards to process them in hardware, resulting in a dramatic
//       performance gain.
//-----------------------------------------------------------------------------
HRESULT InitVB()
{
	// 描画する三角形のために頂点を初期化する。
	CUSTOMVERTEX vertices[] =
	{
		{ 150.0f, 50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color
		{ 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
		{ 50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
		{ 150.0f, 50.0f, 0.5f, 1.0f, 0xff00ffff, },
	};

	// 頂点バッファを作成する.
	// カスタムvertexを使用して作成することを宣言する.
	// Create the vertex buffer. Here we are allocating enough memory
	// (from the default pool) to hold all our 3 custom vertices. We also
	// specify the FVF, so the vertex buffer knows what data it contains.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	// 作成した頂点バッファが途中で書き換えられたりしないように、ロックを行う.
	// ロックを行うことでバッファへのアクセスが可能になる.
	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
	// gain access to the vertices. This mechanism is required becuase vertex
	// buffers may be in device memory.
	VOID* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// @brief  グローバル変数群の解放.
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if (g_pVB != NULL)
		g_pVB->Release();

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
		// streamを利用してvertex buffer内の頂点を描画する。.
		// Draw the triangles in the vertex buffer. This is broken into a few
		// steps. We are passing the vertices down a "stream", so first we need
		// to specify the source of that stream, which is our vertex buffer. Then
		// we need to let D3D know what vertex shader to use. Full, custom vertex
		// shaders are an advanced topic, but in most cases the vertex shader is
		// just the FVF, so that D3D knows what type of vertices we are dealing
		// with. Finally, we call DrawPrimitive() which does the actual rendering
		// of our geometry (in this case, just one triangle).
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

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
		// 頂点バッファ初期化
		if (SUCCEEDED(InitVB()))
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
	}

	// ループを抜けたらウインドウを削除する.
	UnregisterClass("D3D Tutorial", wc.hInstance);
	return 0;
}
