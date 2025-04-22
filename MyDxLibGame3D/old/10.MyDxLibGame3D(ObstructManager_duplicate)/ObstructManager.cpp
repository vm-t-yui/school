//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "ObstructManager.h"
#include "ObstructBase.h"
#include "ObstructStatic.h"
#include "ObstructFloat.h"

/// <summary>
/// コンストラクタ
/// </summary>
ObstructManager::ObstructManager()
	: obstructs					{ NULL }
	, floatModelSourceHandle	(-1)
	, staticModelSourceHandle	(-1)
{
	// 処理なし
}

/// <summary>
/// デストラクタ
/// </summary>
ObstructManager::~ObstructManager()
{
	// 処理なし
}

/// <summary>
/// 障害物生成
/// </summary>
void ObstructManager::CreateObstructs()
{
	floatModelSourceHandle = MV1LoadModel("data/model/obstructFloat/obstructFloat.pmx");
	staticModelSourceHandle = MV1LoadModel("data/model/obstructStatic/obstructStatic.pmd");
	obstructs[0] = new ObstructStatic(staticModelSourceHandle);
	obstructs[1] = new ObstructFloat(floatModelSourceHandle);
	obstructs[2] = new ObstructStatic(staticModelSourceHandle);
	for (int i = 3; i < ObstructNum; ++i)
	{
		obstructs[i] = new ObstructStatic(staticModelSourceHandle);
	}

	// 障害物の位置を初期化
	float band = 10.0f;
	for (int i = 0; i < ObstructNum; ++i)
	{
		obstructs[i]->SetPos(VGet(band * (i - ObstructNum * 0.5f), 0, -1.0f));
	}
}

/// <summary>
/// 障害物削除
/// </summary>
void ObstructManager::DestroyObstructs()
{
	for (int i = 0; i < ObstructNum; ++i)
	{
		delete(obstructs[i]);
		obstructs[i] = NULL;
	}
	MV1DeleteModel(staticModelSourceHandle);
	MV1DeleteModel(floatModelSourceHandle);
}

/// <summary>
/// 更新
/// </summary>
void ObstructManager::Update()
{
	for (auto& obstruct : obstructs)
	{
		if (obstruct != NULL)
		{
			obstruct->Update();
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void ObstructManager::Draw()
{
	for (auto& obstruct : obstructs)
	{
		if (obstruct != NULL)
		{
			obstruct->Draw();
		}
	}
}