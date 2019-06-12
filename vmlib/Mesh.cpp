//-----------------------------------------------------------------------------
// @brief  メッシュクラス.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------
#include "vmlib.h"
#include "Mesh.h"
#include "Device.h"

//-----------------------------------------------------------------------------
// @brief  メッシュクラス.
//-----------------------------------------------------------------------------
Mesh::Mesh()
{
	// 処理なし.
}

//-----------------------------------------------------------------------------
// @brief コンストラクタ.
//-----------------------------------------------------------------------------
Mesh::~Mesh()
{
	Finalize();
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
void Mesh::Initialize(const LPCSTR meshPath, const LPCSTR meshFileName)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;

	// フルパス.
	std::string meshFullPath = meshPath;
	meshFullPath += meshFileName;
	if (FAILED(D3DXLoadMeshFromX(meshFullPath.c_str(), D3DXMESH_SYSTEMMEM,
		GetDxDevice(), NULL,
		&pD3DXMtrlBuffer, NULL, &dwNumMaterials,
		&pMesh)))
	{
        LogAssert("Meshの生成に失敗しました。:%s\n", meshFullPath.c_str());
	}

	// We need to extract the material properties and texture names from the 
	// pD3DXMtrlBuffer
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		// Copy the material
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;

		pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// Create the texture
			// マテリアルに設定されている情報からテクスチャをロードする.
			std::string textureFullPath = meshPath;
			textureFullPath += d3dxMaterials[i].pTextureFilename;
			if (FAILED(D3DXCreateTextureFromFileA(GetDxDevice(),
				textureFullPath.c_str(),
				&pMeshTextures[i])))
			{
                LogAssert("テクスチャの生成に失敗しました。%s\n", textureFullPath.c_str());
			}
		}
	}

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();
}

//-----------------------------------------------------------------------------
// @brief  終了処理.
//-----------------------------------------------------------------------------
void Mesh::Finalize()
{
	if (pMeshMaterials != NULL)
	{
		delete[] pMeshMaterials;
		pMeshMaterials = NULL;
	}

	if (pMeshTextures)
	{
		for (DWORD i = 0; i < dwNumMaterials; i++)
		{
			if (pMeshTextures[i])
				pMeshTextures[i]->Release();
		}
		delete[] pMeshTextures;
		pMeshTextures = NULL;
	}

	if (pMesh != NULL)
	{
		pMesh->Release();
		pMesh = NULL;
	}
}

//-----------------------------------------------------------------------------
// @brief  ワールドマトリクスをセットする.
//-----------------------------------------------------------------------------
void Mesh::SetWorldMatrices()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
    D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
    GetDxDevice()->SetTransform(D3DTS_WORLD, &matWorld);
}

//-----------------------------------------------------------------------------
// @brief  レンダリング.
//-----------------------------------------------------------------------------
void Mesh::Render()
{
	// Meshes are divided into subsets, one for each material. Render them in
	// a loop
	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		// Set the material and texture for this subset
		GetDxDevice()->SetMaterial(&pMeshMaterials[i]);
		GetDxDevice()->SetTexture(0, pMeshTextures[i]);

		// Draw the mesh subset
		pMesh->DrawSubset(i);
	}
}

