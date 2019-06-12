//-----------------------------------------------------------------------------
// @brief  スプライト.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include <cassert>
#include <d3dx9.h>
#include "Sprite.h"
#include "Device.h"
#include "stdio.h"

//-----------------------------------------------------------------------------
// @brief 頂点描画に使用されるカスタム頂点構造体.
//-----------------------------------------------------------------------------
struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;
	D3DCOLOR    color;
	float       tu, tv;
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)		// D3DFVF_TEX1が増えている。CUSTOMVERTEXを変えたらここも合わせるのを忘れずに.

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Sprite::Sprite()
{
	// 処理なし.
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Sprite::~Sprite()
{
	Finalize();
}

//-----------------------------------------------------------------------------
// @brief  初期化.
//-----------------------------------------------------------------------------
void Sprite::Initialize(const Device& device, const LPCSTR texPath)
{
	// テクスチャのロード.
	if (D3DXCreateTextureFromFile(device.GetDevice(), texPath, &pTexture))
	{
		printf("textureの作成に失敗しました。\n");
		assert(0);
	}

	// Initialize three vertices for rendering a sprite
	const float size = 2.0f;
	// D3DPT_TRIANGLESTRIPにあわせて.
	CUSTOMVERTEX vertices[] =
	{
		{ { -size, size, 1.0f }, 0xffffffff, 0, 0 },    //左上
		{ { size, size, 1.0f }, 0xffffffff, 1, 0 },		//右上
		{ { -size, -size, 1.0f }, 0xffffffff, 0, 1 },   //左下
		{ { size, -size, 1.0f }, 0xffffffff, 1, 1 },    //右下
	};

	// Create the vertex buffer.
	if (FAILED(device.GetDevice()->CreateVertexBuffer(
		5 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT,
		&pVB,
		NULL)))
	{
		printf("vertex bufferの作成に失敗しました。\n");
		assert(0);
	}
	else
	{
		// Fill the vertex buffer.
		VOID* pVertices;
		if (FAILED(pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		{
			printf("vertex bufferのロックに失敗しました。\n");
			assert(0);
		}
		else
		{
			memcpy(pVertices, vertices, sizeof(vertices));
			pVB->Unlock();
		}
	}
}

//-----------------------------------------------------------------------------
// @brief  終了処理.
//-----------------------------------------------------------------------------
void Sprite::Finalize()
{
	if (pTexture != NULL)
	{
		pTexture->Release();
		pTexture = NULL;
	}
	if (pVB != NULL)
	{
		pVB->Release();
		pVB = NULL;
	}
}

//-----------------------------------------------------------------------------
// @brief  マトリクスのセットアップ.
//-----------------------------------------------------------------------------
void Sprite::SetWorldMatrices(const Device& device)
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	//UINT iTime = (timeGetTime() / 3) % 1000;
	//FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	//D3DXMatrixRotationY(&matWorld, fAngle);
	//device.GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
}

//-----------------------------------------------------------------------------
// @brief  レンダリング.
//-----------------------------------------------------------------------------
void Sprite::Render(const Device& device)
{
	// streamを利用してvertex buffer内の頂点を描画する。.
	// Draw the sprites in the vertex buffer. This is broken into a few
	// steps. We are passing the vertices down a "stream", so first we need
	// to specify the source of that stream, which is our vertex buffer. Then
	// we need to let D3D know what vertex shader to use. Full, custom vertex
	// shaders are an advanced topic, but in most cases the vertex shader is
	// just the FVF, so that D3D knows what type of vertices we are dealing
	// with. Finally, we call DrawPrimitive() which does the actual rendering
	// of our geometry (in this case, just one sprite).
	const LPDIRECT3DDEVICE9 d3Device = device.GetDevice();
	d3Device->SetTexture(0, pTexture);
	d3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	d3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d3Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	d3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	d3Device->SetStreamSource(0, pVB, 0, sizeof(CUSTOMVERTEX));
	d3Device->SetFVF(D3DFVF_CUSTOMVERTEX);
	d3Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);		// primitiveCountは「三角形の数」。四角形なので２に増えている.
}