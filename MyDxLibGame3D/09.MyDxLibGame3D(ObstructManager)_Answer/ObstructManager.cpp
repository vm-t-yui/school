//-----------------------------------------------------------------------------
// @brief  障害物マネージャー.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "ObstructManager.h"
#include "ObstructBase.h"
#include "ObstructStatic.h"
#include "ObstructFloat.h"

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
ObstructManager::ObstructManager()
{
	for (int i = 0; i < OBSTRUCT_NUM; ++i)
	{
		obstructs[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
ObstructManager::~ObstructManager()
{
	// 処理なし.
}

//-----------------------------------------------------------------------------
// @brief  障害物生成.
//-----------------------------------------------------------------------------
void ObstructManager::CreateObstructs()
{
	// ３Ｄモデルの読み込み
	sourceModelHandleStatic = MV1LoadModel("data/model/obstructStatic/obstructStatic.pmd");
	if (sourceModelHandleStatic < 0)
	{
		printfDx("sourceModelHandleStatic:データ読み込みに失敗");
	}
	sourceModelHandleFloat = MV1LoadModel("data/model/obstructFloat/obstructFloat.pmx");
	if (sourceModelHandleFloat < 0)
	{
		printfDx("ObstructFloat:データ読み込みに失敗");
	}

	obstructs[0] = new ObstructStatic(sourceModelHandleStatic);
	obstructs[1] = new ObstructFloat(sourceModelHandleFloat);
	obstructs[2] = new ObstructStatic(sourceModelHandleStatic);

	// 障害物の位置を初期化.
	float band = 10.0f;
	for (int i = 0; i < OBSTRUCT_NUM; ++i)
	{
		obstructs[i]->SetPos(VGet(-band + (band * i), 0, -1.0f));
	}
}

//-----------------------------------------------------------------------------
// @brief  障害物削除.
//-----------------------------------------------------------------------------
void ObstructManager::DestroyObstructs()
{
	for (int i = 0; i < OBSTRUCT_NUM; ++i)
	{
		delete(obstructs[i]);
		obstructs[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void ObstructManager::Update()
{
	for (int i = 0; i < OBSTRUCT_NUM; ++i)
	{
		if (obstructs[i] != NULL)
		{
			obstructs[i]->Update();
		}
	}
}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void ObstructManager::Draw()
{
	for (int i = 0; i < OBSTRUCT_NUM; ++i)
	{
		if (obstructs[i] != NULL)
		{
			obstructs[i]->Draw();
		}
	}
}