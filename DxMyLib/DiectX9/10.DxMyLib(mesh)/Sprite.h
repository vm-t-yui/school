//-----------------------------------------------------------------------------
// @brief  スプライト.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#pragma once
#include <d3d9.h>

class Device;

class Sprite final
{
public:
	Sprite();										// コンストラクタ.
	~Sprite();									// デストラクタ.

	void Initialize(const Device& devicev, const LPCSTR texPath);			// 初期化.
	void Finalize();								// 終了処理.
	void SetWorldMatrices(const Device& device);
	void Render(const Device& device);

private:
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;           // Buffer to hold vertices
	LPDIRECT3DTEXTURE9      pTexture = NULL;
};

