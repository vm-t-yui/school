//-----------------------------------------------------------------------------
// @brief  VMメッシュクラス.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include <d3d9.h>
#include <d3dx9mesh.h>

class Device;
class VMParts;
class VMTexture;

class VMMesh final
{
public:
    VMMesh();									// コンストラクタ.
	~VMMesh();									// デストラクタ.

	void Initialize(const LPCSTR meshPath, const LPCSTR meshFileName);			// 初期化.
	void Finalize();							// 終了処理.
	void SetWorldMatrices();
	void Render();
    int  GetPartsNum() const { return (int)partsList.size(); }
    const VMTexture* GetTexture() const { return texture; }

private:
    std::vector<VMParts*>   partsList;
    VMTexture*              texture;
};
