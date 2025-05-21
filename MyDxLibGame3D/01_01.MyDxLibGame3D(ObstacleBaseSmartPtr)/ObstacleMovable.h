// 2025 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// 障害物：移動する
/// </summary>
class ObstacleMovable : public ObstacleBase
{
public:
	ObstacleMovable();				// コンストラクタ
	virtual ~ObstacleMovable();		// デストラクタ

    void Update();					// 更新
    void Draw() const;				// 描画

    // モデルハンドルの取得
    int GetModelHandle() const { return modelHandle; }

    // ポジションのgetter/setter
    const VECTOR& GetPos() const { return pos; }
    void SetPos(const VECTOR set) { pos = set; }

private:
    VECTOR	pos;			// ポジション
	float   rad;			// sin移動用のラジアン関数
};

