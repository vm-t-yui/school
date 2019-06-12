//-----------------------------------------------------------------------------
// @brief  障害物マネージャー.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _OBSTRUCT_MANAGER_H_
#define _OBSTRUCT_MANAGER_H_

class ObstructBase;

// 障害物の数.
#define OBSTRUCT_NUM 1000

class ObstructManager final
{
public:
	ObstructManager();				// コンストラクタ.
	~ObstructManager();				// デストラクタ.

	void CreateObstructs();			// 障害物生成.
	void DestroyObstructs();		// 障害物削除.

	void Update();					// 更新.
	void Draw();					// 描画.

private:
	ObstructBase* obstructs[OBSTRUCT_NUM];

	int floatModelSourceHandle;		// 浮遊モデルの大本のハンドル.
	int staticModelSourceHandle;	// 静止モデルの大本のハンドル.
};

#endif // _OBSTRUCT_MANAGER_H_