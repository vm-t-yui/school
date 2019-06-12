//----------------------------------------
//@brief メッシュクラス
//------------------------------------

#ifndef _MESH_H_
#define _MESH_H_

#include "Device.h"
#include <d3dx9.h>

class Mesh
{
public:
	Mesh();
	~Mesh();
	HRESULT InitD3D(HWND hWnd);
	HRESULT InitGiometry();
	void SetUpMatrices();
	void Render();

private:
	LPDIRECT3D9         g_pD3D; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9   g_pd3dDevice; // Our rendering device
	LPD3DXMESH          g_pMesh;
	D3DMATERIAL9*       g_pMeshMaterials;
	LPDIRECT3DTEXTURE9* g_pMeshTextures;
	DWORD               g_dwNumMaterials;
};


#endif