//-----------------------------------------------------------------------------
// @brief  障害物マネージャー.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _OBSTRUCT_MANAGER_H_
#define _OBSTRUCT_MANAGER_H_

class ObstructBase;

// 障害物の数.
#define OBSTRUCT_NUM 3

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

	// 基となるモデルハンドル.
	int sourceModelHandleStatic;
	int sourceModelHandleFloat;
};

#endif // _OBSTRUCT_MANAGER_H_