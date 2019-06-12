//-----------------------------------------------------------------------------
// @brief  グラフィックデバイス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <d3d9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )


class Device final
{
public:
    Device();				        // コンストラクタ.
    ~Device();				        // デストラクタ.

    bool Initialize(HWND hWnd);     // 初期化.
    void Finalize();                // 終了処理.

    const LPDIRECT3DDEVICE9 GetDevice() { return pd3dDevice; }

private:
    LPDIRECT3D9             pD3D;          // Used to create the D3DDevice
    LPDIRECT3DDEVICE9       pd3dDevice;    // Our rendering device
};

#endif // _DEVICE_H_