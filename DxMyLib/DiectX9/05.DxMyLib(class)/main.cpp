//-----------------------------------------------------------------------------
// @brief  三角形表示のチュートリアル.
// Copyright (c) 2016 Takeru Yui.
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include "Device.h"

//-----------------------------------------------------------------------------
// グローバル変数群.
//-----------------------------------------------------------------------------
Device* g_device = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;           // Buffer to hold vertices

//-----------------------------------------------------------------------------
// @brief 頂点描画に使用されるカスタム頂点構造体.
//-----------------------------------------------------------------------------
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The transformed position for the vertex
    DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

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
    // Initialize three vertices for rendering a triangle
    CUSTOMVERTEX g_Vertices[] =
    {
        { -1.0f,-1.0f, 0.0f, 0xffff0000, },
        { 1.0f,-1.0f, 0.0f, 0xff0000ff, },
        { 0.0f, 1.0f, 0.0f, 0xffffffff, },
    };

    // Create the vertex buffer.
    if (FAILED(g_device->GetDevice()->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
        0, D3DFVF_CUSTOMVERTEX,
        D3DPOOL_DEFAULT, &g_pVB, NULL)))
    {
        return E_FAIL;
    }

    // Fill the vertex buffer.
    VOID* pVertices;
    if (FAILED(g_pVB->Lock(0, sizeof(g_Vertices), (void**)&pVertices, 0)))
        return E_FAIL;
    memcpy(pVertices, g_Vertices, sizeof(g_Vertices));
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

    if (g_device != NULL)
    {
        g_device->Finalize();
        delete g_device;
        g_device = NULL;
    }
}



//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
VOID SetupMatrices()
{
    // For our world matrix, we will just rotate the object about the y-axis.
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity(&matWorld);

    // Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
    // every 1000 ms. To avoid the loss of precision inherent in very high 
    // floating point numbers, the system time is modulated by the rotation 
    // period before conversion to a radian angle.
    UINT iTime = timeGetTime() % 1000;
    FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
    D3DXMatrixRotationY(&matWorld, fAngle);
    g_device->GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);

    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the
    // origin, and define "up" to be in the y-direction.
    D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
    D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
    g_device->GetDevice()->SetTransform(D3DTS_VIEW, &matView);

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
    g_device->GetDevice()->SetTransform(D3DTS_PROJECTION, &matProj);
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
        // マトリクス設定.
        SetupMatrices();

        // streamを利用してvertex buffer内の頂点を描画する。.
        // Draw the triangles in the vertex buffer. This is broken into a few
        // steps. We are passing the vertices down a "stream", so first we need
        // to specify the source of that stream, which is our vertex buffer. Then
        // we need to let D3D know what vertex shader to use. Full, custom vertex
        // shaders are an advanced topic, but in most cases the vertex shader is
        // just the FVF, so that D3D knows what type of vertices we are dealing
        // with. Finally, we call DrawPrimitive() which does the actual rendering
        // of our geometry (in this case, just one triangle).
        g_device->GetDevice()->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
        g_device->GetDevice()->SetFVF(D3DFVF_CUSTOMVERTEX);
        g_device->GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

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
