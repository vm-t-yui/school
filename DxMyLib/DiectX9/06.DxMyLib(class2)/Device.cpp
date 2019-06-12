//-----------------------------------------------------------------------------
// @brief  グラフィックデバイス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Device.h"

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Device::Device()
    : pD3D(NULL)
     ,pd3dDevice(NULL)
{
    // 処理なし.
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Device::~Device()
{
    Finalize();
}

//-----------------------------------------------------------------------------
// @brief  初期化.
//-----------------------------------------------------------------------------
bool Device::Initialize(HWND hWnd)
{
    // D3Dオブジェクトを生成.
    if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
        return false;

    // D3Dデバイス生成用の宣言.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    // D3Dデバイス生成
    if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &pd3dDevice)))
    {
        return false;
    }

    // 通常、デバイスの状態設定はここで行われる.
    // Device state would normally be set here

    // カリングをオフにする。これによって裏表両方見える状態に.
    // Turn off culling, so we see the front and back of the triangle
    pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    // ライティングを無効に.
    // Turn off D3D lighting, since we are providing our own vertex colors
    pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    return true;
}

//-----------------------------------------------------------------------------
// @brief  終了処理.
//-----------------------------------------------------------------------------
void Device::Finalize()
{
    if (pd3dDevice != NULL)
    {
        pd3dDevice->Release();
    }
    if (pD3D != NULL)
    {
        pD3D->Release();
    }
}