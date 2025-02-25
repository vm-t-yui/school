// 2025 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// 障害物:動かない
/// </summary>
class ObstacleKinematic
{
public:
	ObstacleKinematic();				// コンストラクタ
	virtual ~ObstacleKinematic();		// デストラクタ

	void Update();					// 更新
    void Draw() const;					// 描画

    // モデルハンドルの取得
    int GetModelHandle() const { return modelHandle; }

    // ポジションのgetter/setter
    const VECTOR& GetPos() const { return pos; }
    void SetPos(const VECTOR set) { pos = set; }

private:
    int		modelHandle;	// モデルハンドル
    VECTOR	pos;			// ポジション
};

