//-----------------------------------------------------------------------------
// @brief  三角形.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include <cassert>
#include <d3dx9.h>
#include "Triangle.h"
#include "Device.h"
#include "stdio.h"

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
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Triangle::Triangle()
{
	// 処理なし.
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Triangle::~Triangle()
{
	Finalize();
}

//-----------------------------------------------------------------------------
// @brief  初期化.
//-----------------------------------------------------------------------------
void Triangle::Initialize(const Device& device)
{
	// Initialize three vertices for rendering a triangle
	CUSTOMVERTEX vertices[] =
	{
		{ 0.0f, 1.0f, 0.0f, 0xffffffff, },
		{ 1.0f, -1.0f, 0.0f, 0xff0000ff, },
		{ -1.0f, -1.0f, 0.0f, 0xffff0000, },
	};

	// Create the vertex buffer.
	if (FAILED(device.GetDevice()->CreateVertexBuffer(
		3 * sizeof(CUSTOMVERTEX),
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
void Triangle::Finalize()
{
	if (pVB != NULL)
	{
		pVB->Release();
		pVB = NULL;
	}
}

//-----------------------------------------------------------------------------
// @brief  マトリクスのセットアップ.
//-----------------------------------------------------------------------------
void Triangle::SetWorldMatrices(const Device& device)
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
	device.GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
}

//-----------------------------------------------------------------------------
// @brief  レンダリング.
//-----------------------------------------------------------------------------
void Triangle::Render(const Device& device)
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
	device.GetDevice()->SetStreamSource(0, pVB, 0, sizeof(CUSTOMVERTEX));
	device.GetDevice()->SetFVF(D3DFVF_CUSTOMVERTEX);
	device.GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}