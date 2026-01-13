//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#pragma once

/// <summary>
/// 障害物基底クラス
/// </summary>
class ObstructBase abstract
{
public:
	ObstructBase(int sourceModelHandle);	// コンストラクタ
	virtual ~ObstructBase();				// デストラクタ

	//virtual void Update() = 0;			// 更新
	virtual void Update() abstract;			// 更新
	virtual void Draw();					// 描画

	// モデルハンドルの取得
	int GetModelHandle() const { return modelHandle; }

	// ポジションのgetter/setter
	const VECTOR& GetPos() const { return pos; }
	void SetPos(const VECTOR set) { pos = set; }

protected:
	int		modelHandle;					// モデルハンドル
	VECTOR	pos;							// ポジション
};
