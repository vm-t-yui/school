//-----------------------------------------------------------------------------
// @brief  三角形.
// 2016 Takeru Yui All Rights Reserved.
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

	void Initialize(const Device& device);			// 初期化.
	void Finalize();								// 終了処理.
	void SetWorldMatrices(const Device& device);
	void Render(const Device& device);

private:
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;           // Buffer to hold vertices
};

#endif // _TRIANGLE_H_