//-----------------------------------------------------------------------------
// @brief  メッシュ.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include <d3d9.h>
#include <d3dx9mesh.h>

class Device;

class Mesh final
{
public:
	Mesh();										// コンストラクタ.
	~Mesh();									// デストラクタ.

	void Initialize(const LPCSTR meshPath, const LPCSTR meshFileName);			// 初期化.
	void Finalize();								// 終了処理.
	void SetWorldMatrices();
	void Render();

private:
	LPD3DXMESH          pMesh = NULL; // Our mesh object in sysmem
	D3DMATERIAL9*       pMeshMaterials = NULL; // Materials for our mesh
	LPDIRECT3DTEXTURE9* pMeshTextures = NULL; // Textures for our mesh
	DWORD               dwNumMaterials = 0L;   // Number of mesh materials
};

