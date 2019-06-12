//-----------------------------------------------------------------------------
// @brief  テクスチャクラス.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

#include <d3dx9.h>

class VMTexture final
{
public:
    VMTexture() {}
    ~VMTexture();

    void Load(const LPCSTR path);
    void Unload();
    const LPDIRECT3DTEXTURE9 GetTexture() const { return texture; }

private:
    LPDIRECT3DTEXTURE9      texture = NULL;
};
