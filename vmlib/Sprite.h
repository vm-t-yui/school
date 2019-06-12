//-----------------------------------------------------------------------------
// @brief  スプライト.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include <d3d9.h>

class Device;

class Sprite final
{
public:
	Sprite();										// コンストラクタ.
	~Sprite();								    	// デストラクタ.

	void Initialize(const LPCSTR texPath);			// 初期化.
	void Finalize();								// 終了処理.
	void SetWorldMatrices();
	void Render();

private:
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;           // Buffer to hold vertices
	LPDIRECT3DTEXTURE9      pTexture = NULL;
};

