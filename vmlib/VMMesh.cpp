//-----------------------------------------------------------------------------
// @brief  VMメッシュクラス.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------
#include "vmlib.h"
#include "VMMesh.h"
#include "Device.h"
#include "VMParts.h"
#include "VMTexture.h"

//-----------------------------------------------------------------------------
// @brief コンストラクタ.
//-----------------------------------------------------------------------------
VMMesh::VMMesh()
    : texture(NULL)
{
	// 処理なし.
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
VMMesh::~VMMesh()
{
	Finalize();
}

//-----------------------------------------------------------------------------
// @brief  初期化.
//-----------------------------------------------------------------------------
void VMMesh::Initialize(const LPCSTR meshPath, const LPCSTR meshFileName)
{
	// フルパス.
	std::string meshFullPath = meshPath;
    meshFullPath += meshFileName;
    meshFullPath += ".fbx";
	
    // FBXからメッシュとマテリアルをロード.
    FbxScene* scene = GetFbxManager()->LoadFbx(meshFullPath.c_str());

    if (scene == NULL)
    {
        LogAssert("fbx load failed. path:%s\n", meshFullPath.c_str());
    }

    // ノードからメッシュだけを取り出す.
    std::vector<FbxMesh*> meshlist;
    FbxNode* rootNode = scene->GetRootNode();
    int childCount = rootNode->GetChildCount(true);
    for (int i = 0; i < childCount; i++)
    {
        FbxNode* node = rootNode->GetChild(i);
        if (node)
        {
#if _DEBUG
            FbxNodeAttribute* nodeAttr = node->GetNodeAttribute();
            if (nodeAttr)
            {
                fbxsdk::FbxNodeAttribute::EType type = nodeAttr->GetAttributeType();
                if (type == FbxNodeAttribute::eMesh)
                {
                    meshlist.push_back((FbxMesh*)nodeAttr);
                }
            }
#else
            FbxMesh* mesh = VMFbxManager::GetMesh(node);
            if (mesh)
            {
                meshlist.push_back(mesh);
            }
#endif
        }
    }
    if (meshlist.size() == 0)
    {
        Log("mesh nothing. path:%s\n", meshFullPath.c_str());
    }

    partsList.clear();
    for (std::vector<FbxMesh*>::iterator ite = meshlist.begin(); ite != meshlist.end(); ++ite)
    {
        partsList.push_back(new VMParts(this, *ite ));
    }

    // FIXME:ちゃんとした管理方法.
    // とりあえず仮の仕様として、同名のpngテクスチャが存在した場合それをテクスチャとして使用する形に.
	std::string texPath = meshPath;
    texPath += meshFileName;
    texPath += ".png";
    texture = new VMTexture();
    texture->Load(texPath.c_str());
}

//-----------------------------------------------------------------------------
// @brief  終了処理.
//-----------------------------------------------------------------------------
void VMMesh::Finalize()
{
    for (std::vector<VMParts*>::iterator ite = partsList.begin(); ite != partsList.end(); ++ite)
    {
        delete (*ite);
    }
    partsList.clear();

    if (texture)
    {
        texture->Unload();
        delete texture;
        texture = NULL;
    }
}

//-----------------------------------------------------------------------------
// @brief  ワールドマトリクスをセットする.
//-----------------------------------------------------------------------------
void VMMesh::SetWorldMatrices()
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
void VMMesh::Render()
{
    for (std::vector<VMParts*>::iterator ite = partsList.begin(); ite != partsList.end(); ++ite)
    {
        (*ite)->Render();
    }
}

