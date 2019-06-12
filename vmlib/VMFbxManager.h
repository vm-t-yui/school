//-----------------------------------------------------------------------------
// @brief  FBXのマネージャー.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "fbxsdk.h"

class VMFbxManager
{
public:
    VMFbxManager();         // コンストラクタ.
    ~VMFbxManager();        // デストラクタ.

    void Initialize();      // 初期化.
    void Finalize();        // 終了処理.

    FbxScene* LoadFbx(const LPCSTR path);    // FBXをロード.

    // メッシュを取得する.
    static FbxMesh* GetMesh(FbxNode* node);

private:
    fbxsdk::FbxManager*     manager;
};

