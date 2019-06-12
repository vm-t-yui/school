//-----------------------------------------------------------------------------
// @brief  三角形.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <d3d9.h>

class Device;

class Triangle final
{
public:
	Triangle();										// コンストラクタ.
	~Triangle();									// デストラクタ.

	void Initialize();			// 初期化.
	void Finalize();								// 終了処理.
	void SetWorldMatrices();
	void Render();

private:
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;           // Buffer to hold vertices
};

#endif // _TRIANGLE_H_