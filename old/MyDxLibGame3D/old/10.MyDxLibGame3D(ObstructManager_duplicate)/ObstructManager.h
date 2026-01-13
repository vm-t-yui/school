//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

class ObstructBase;

/// <summary>
/// 障害物マネージャー
/// </summary>
class ObstructManager final
{
public:
	ObstructManager();				// コンストラクタ
	~ObstructManager();				// デストラクタ

	void CreateObstructs();			// 障害物生成
	void DestroyObstructs();		// 障害物削除

	void Update();					// 更新
	void Draw();					// 描画

private:
	// 障害物の数
	static constexpr int ObstructNum = 1000;

	ObstructBase* obstructs[ObstructNum];

	int floatModelSourceHandle;		// 浮遊モデルの大本のハンドル
	int staticModelSourceHandle;	// 静止モデルの大本のハンドル
};