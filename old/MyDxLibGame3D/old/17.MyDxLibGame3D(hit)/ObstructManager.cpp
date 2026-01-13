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
	: floatModelSourceHandle(-1)
	, staticModelSourceHandle(-1)
{
	for (int i = 0; i < LINE_OBSTRUCT_RAW; i++)
	{
		for (int j = 0; j < LINE_OBSTRUCT_COL; j++)
		{
			obstructs[i][j] = NULL;
		}
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
	floatModelSourceHandle = MV1LoadModel("data/model/obstructFloat/obstructFloat.pmx");
	staticModelSourceHandle = MV1LoadModel("data/model/obstructStatic/obstructStatic.pmd");

	// 障害物の配置図データとしてのlinemapを用意する.
	int line1000001[LINE_OBSTRUCT_COL] = { 1,0,0,0,0,0,1 };
	int line1000021[LINE_OBSTRUCT_COL] = { 1,0,0,0,0,2,1 };
	int line1000201[LINE_OBSTRUCT_COL] = { 1,0,0,0,2,0,1 };
	int line1002001[LINE_OBSTRUCT_COL] = { 1,0,0,2,0,0,1 };
	int line1020001[LINE_OBSTRUCT_COL] = { 1,0,2,0,0,0,1 };
	int line1200001[LINE_OBSTRUCT_COL] = { 1,2,0,0,0,0,1 };
	int line1220001[LINE_OBSTRUCT_COL] = { 1,2,2,0,0,0,1 };
	int line1022001[LINE_OBSTRUCT_COL] = { 1,0,2,2,0,0,1 };
	int line1002201[LINE_OBSTRUCT_COL] = { 1,0,0,2,2,0,1 };
	int line1000221[LINE_OBSTRUCT_COL] = { 1,0,0,0,2,2,1 };
	int line1202001[LINE_OBSTRUCT_COL] = { 1,2,0,2,0,0,1 };
	int line1020201[LINE_OBSTRUCT_COL] = { 1,0,2,0,2,0,1 };
	int line1002021[LINE_OBSTRUCT_COL] = { 1,0,0,2,0,2,1 };
	int line1200201[LINE_OBSTRUCT_COL] = { 1,2,0,0,2,0,1 };
	int line1020021[LINE_OBSTRUCT_COL] = { 1,0,2,0,0,2,1 };
	int line1200021[LINE_OBSTRUCT_COL] = { 1,2,0,0,0,2,1 };
	int line1222001[LINE_OBSTRUCT_COL] = { 1,2,2,2,0,0,1 };
	int line1022201[LINE_OBSTRUCT_COL] = { 1,0,2,2,2,0,1 };
	int line1002221[LINE_OBSTRUCT_COL] = { 1,0,0,2,2,2,1 };
	int line1202201[LINE_OBSTRUCT_COL] = { 1,2,0,2,2,0,1 };
	int line1200221[LINE_OBSTRUCT_COL] = { 1,2,0,0,2,2,1 };
	int line1220021[LINE_OBSTRUCT_COL] = { 1,2,2,0,0,2,1 };
	int line1022221[LINE_OBSTRUCT_COL] = { 1,0,2,2,2,2,1 };
	int line1202221[LINE_OBSTRUCT_COL] = { 1,2,0,2,2,2,1 };
	int line1220221[LINE_OBSTRUCT_COL] = { 1,2,2,0,2,2,1 };
	int line1222021[LINE_OBSTRUCT_COL] = { 1,2,2,2,0,2,1 };
	int line1222201[LINE_OBSTRUCT_COL] = { 1,2,2,2,2,0,1 };

	int* lineMap[LINE_OBSTRUCT_RAW] =
	{
		line1222201,	// 1
		line1000001,	// 2
		line1000001,	// 3
		line1000201,	// 4
		line1000201,	// 5
		line1000001,	// 6
		line1000001,	// 7
		line1000001,	// 8
		line1000001,	// 9
		line1000001,	// 10
		line1002221,	// 11
		line1000001,	// 12
		line1000001,	// 13
		line1000001,	// 14
		line1000001,	// 15
		line1000201,	// 16
		line1000001,	// 17
		line1222001,	// 18
		line1222001,	// 19
		line1000001,	// 20
		line1000001,	// 21
		line1000001,	// 22
		line1022201,	// 23
		line1000201,	// 24
		line1000201,	// 25
		line1000001,	// 26
		line1000001,	// 27
		line1000001,	// 28
		line1000001,	// 29
		line1000021,	// 30
	};

	for (int i = 0; i < LINE_OBSTRUCT_RAW; i++)
	{
		int* nowLine = lineMap[i];
		for (int j = 0; j < LINE_OBSTRUCT_COL; j++)
		{
			// linemapに入っている数値によって障害物の種類を決定する.
			if (nowLine[j] == 1)
			{
				obstructs[i][j] = new ObstructStatic(staticModelSourceHandle);
			}
			else if(nowLine[j] == 2)
			{
				obstructs[i][j] = new ObstructFloat(floatModelSourceHandle);
			}
			else
			{
				obstructs[i][j] = NULL;
			}

			// 位置の初期化.
			if (obstructs[i][j] != NULL)
			{
				obstructs[i][j]->SetPos(
					VGet(
						(OBSTRUCT_SPACE_W * j) - (OBSTRUCT_SPACE_W * LINE_OBSTRUCT_COL * 0.5f),
						0.0f,
						(OBSTRUCT_SPACE_D * LINE_OBSTRUCT_RAW) - (OBSTRUCT_SPACE_D * i)
						)
					);
			}
		}
	}

}

//-----------------------------------------------------------------------------
// @brief  障害物削除.
//-----------------------------------------------------------------------------
void ObstructManager::DestroyObstructs()
{

	for (int i = 0; i < LINE_OBSTRUCT_RAW; i++)
	{
		for (int j = 0; j < LINE_OBSTRUCT_COL; j++)
		{
			if (obstructs[i][j] != NULL)
			{
				delete(obstructs[i][j]);
				obstructs[i][j] = NULL;
			}
		}
	}
	MV1DeleteModel(staticModelSourceHandle);
	MV1DeleteModel(floatModelSourceHandle);
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void ObstructManager::Update()
{
	for (int i = 0; i < LINE_OBSTRUCT_RAW; i++)
	{
		for (int j = 0; j < LINE_OBSTRUCT_COL; j++)
		{
			if (obstructs[i][j] != NULL)
			{
				obstructs[i][j]->Update();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void ObstructManager::Draw()
{
	for (int i = 0; i < LINE_OBSTRUCT_RAW; i++)
	{
		for (int j = 0; j < LINE_OBSTRUCT_COL; j++)
		{
			if (obstructs[i][j] != NULL)
			{
				obstructs[i][j]->Draw();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// @brief  指定番号の障害物を取得.
//-----------------------------------------------------------------------------
ObstructBase* ObstructManager::GetObstruct(int raw, int col)
{
	return obstructs[raw][col];
}
