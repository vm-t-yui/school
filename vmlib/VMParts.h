//-----------------------------------------------------------------------------
// @brief  VMメッシュのパーツクラス.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

#include <d3dx9.h>

#include "fbxsdk.h"
#include "VMTexture.h"

class VMMesh;

class VMParts final
{
public:
    VMParts(VMMesh* owner, FbxMesh* mesh);
    ~VMParts();

    void Render();   // 描画.

    int GetPolygonCount() const { return polygonCount; }
    int GetVertexCount() const { return vertexCount; }
    int GetIndexCount() const { return indexCount; }

private:
    VMParts() {}

    //void GetBaseTextureMap(FbxSurfaceMaterial* material);
    //void GetTextureMap(FbxSurfaceMaterial* material, const char* pMaterialName);

    //-----------------------------------------------------------------------------
    // @brief 頂点描画に使用されるカスタム頂点構造体.
    //-----------------------------------------------------------------------------
    struct CUSTOMVERTEX
    {
        D3DXVECTOR3 position;   // 頂点座標.
        D3DXVECTOR3 normal;     // 法線方向.
        DWORD       color;      // 色.
        D3DXVECTOR2 uv;         // テクスチャUV座標.
    };
    const VMMesh*           ownerMesh;          // オーナーのメッシュ.
    int                     polygonCount;       // ポリゴン数.
    int*                    polygonSizes;       // ポリゴンサイズ.
    int                     vertexCount;        // 総頂点数.
    LPDIRECT3DVERTEXBUFFER9 d3dVertexBuffer;    // 頂点バッファ.
    CUSTOMVERTEX*           vertices;           // 頂点データ配列.
    int                     indexCount;         // インデックス数.
    LPDIRECT3DINDEXBUFFER9  d3dIndexBuffer;     // インデックスバッファ.
    WORD*                   indices;            // インデックスデータ配列.
    int                     normalLayerCount;   // 法線のセット数.
};
