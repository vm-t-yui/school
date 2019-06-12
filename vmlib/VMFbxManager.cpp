//-----------------------------------------------------------------------------
// @brief  FBXのマネージャー.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------
#include "vmlib.h"
#include "VMFbxManager.h"

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
VMFbxManager::VMFbxManager()
    : manager(NULL)
{
    // 処理なし.
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
VMFbxManager::~VMFbxManager()
{
    Finalize();
}

//-----------------------------------------------------------------------------
// @brief  初期化.
//-----------------------------------------------------------------------------
void VMFbxManager::Initialize()
{
    manager = fbxsdk::FbxManager::Create();
}

//-----------------------------------------------------------------------------
// @brief  終了処理.
//-----------------------------------------------------------------------------
void VMFbxManager::Finalize()
{
    if (manager != NULL)
    {
        manager->Destroy();
        manager = NULL;
    }
}

//-----------------------------------------------------------------------------
// @brief  FBXをロード.
//-----------------------------------------------------------------------------
FbxScene* VMFbxManager::LoadFbx(const LPCSTR path)
{
    assert(manager != NULL);

    FbxScene* ret = NULL;

    fbxsdk::FbxImporter* importer = fbxsdk::FbxImporter::Create(manager, "");
    if (importer == NULL)
    {
        LogAssert("importer Create fail.");
    }

    if (!importer->Initialize(path))
    {
        // importer initialize失敗.
        LogAssert("importer Initialize fail.path:%s\n", path);
    }
    else
    {
        ret = FbxScene::Create(manager, "");
        if (!importer->Import(ret))
        {
            LogAssert("importer Import fail.path:%s\n", path);
        }
    }

    importer->Destroy();

    return ret;
}

//-----------------------------------------------------------------------------
// @brief  Meshを取得する.
//-----------------------------------------------------------------------------
FbxMesh* VMFbxManager::GetMesh(FbxNode* node)
{
    // ノードの属性を取得.
    FbxNodeAttribute* attr = node->GetNodeAttribute();

    if (NULL != attr)
    {
        switch (attr->GetAttributeType()) 
        {
        case FbxNodeAttribute::eMesh:
            return node->GetMesh();
        }
    }

    return NULL;
}