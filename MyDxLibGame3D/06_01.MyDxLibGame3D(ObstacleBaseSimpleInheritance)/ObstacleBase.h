// 2025 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// 障害物クラスの基底
/// </summary>
class ObstacleBase
{
public:
    void Draw() const;					// 描画

    // モデルハンドルの取得
    int GetModelHandle() const { return modelHandle; }

    // ポジションのgetter/setter
    const VECTOR& GetPos() const { return pos; }
    void SetPos(const VECTOR set) { pos = set; }

protected:
    int		modelHandle;	// モデルハンドル
    VECTOR	pos;			// ポジション
};

