// 2025 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// 障害物クラスの基底
/// </summary>
class ObstacleBase abstract // abstractを付けることで、直接使用(new等)を禁止する
{
public:
    ObstacleBase(const char* path);     // コンストラクタ
    ~ObstacleBase();                    // デストラクタ

    void Draw() const;					// 描画

    // 純粋仮想関数
    // void Updateは中身がなく、派生先で中身を作ることを強制する
    //virtual void Update() = 0;			// =0は古い純粋仮想関数宣言
    virtual void Update() abstract;			// 更新

    // モデルハンドルの取得
    int GetModelHandle() const { return modelHandle; }

    // ポジションのgetter/setter
    const VECTOR& GetPos() const { return pos; }
    void SetPos(const VECTOR set) { pos = set; }

protected:
    int		modelHandle;	// モデルハンドル
    VECTOR	pos;			// ポジション
};

