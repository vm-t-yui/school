//-----------------------------------------------------------------------------
// @brief  障害物マネージャー.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _OBSTRUCT_MANAGER_H_
#define _OBSTRUCT_MANAGER_H_

class ObstructBase;

// 障害物の数.
#define LINE_OBSTRUCT_COL 7
#define LINE_OBSTRUCT_RAW 30

#define OBSTRUCT_SPACE_D 20.0f // 障害物の縦間隔.
#define OBSTRUCT_SPACE_W 20.0f // 障害物の横間隔.

class ObstructManager final
{
public:
	ObstructManager();				// コンストラクタ.
	~ObstructManager();				// デストラクタ.

	void CreateObstructs();			// 障害物生成.
	void DestroyObstructs();		// 障害物削除.

	void Update();					// 更新.
	void Draw();					// 描画.

	// 指定番号の障害物を取得.
	ObstructBase* GetObstruct(int raw, int col);

private:
	ObstructBase* obstructs[LINE_OBSTRUCT_RAW][LINE_OBSTRUCT_COL];

	int floatModelSourceHandle;		// 浮遊モデルの大本のハンドル.
	int staticModelSourceHandle;	// 静止モデルの大本のハンドル.
};

#endif // _OBSTRUCT_MANAGER_H_