// 2023 Takeru Yui All Rights Reserved.
#pragma once

class Map;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player final
{
public:
	Player();						// コンストラクタ
	~Player();						// デストラクタ

	void Update(const Map& map);	// 更新
	void Draw();					// 描画

	const VECTOR& GetPos() const { return pos; }

private:
	int		modelHandle;	// モデルハンドル
	VECTOR	pos;			// ポジション
	VECTOR	velocity;		// 移動力
	VECTOR	dir;			// 回転方向
	float	w, h;			// 幅、高さ
	float	fallSpeed;		// プレイヤーの落下速度。ジャンプ時は反転する
	bool	isGround;		// プレイヤーが接地中か
	bool	isHitTop;		// プレイヤーの頭が天井に当たっているか
	
	VECTOR CheckHitWithMap(const Map& map, const VECTOR& velocity);
	bool IsHitWithMapChip(const VECTOR& futurePos, const Map::Chip& mapChip);
	void CheckIsTopHit(const Map& map);
	void CheckIsGround(const Map& map);

	// 静的定数
	// 速度（1=1m、60fps固定として、時速10km）
	// 10000m ÷ 時間 ÷ 分 ÷ 秒 ÷ フレーム
	static constexpr float Speed = static_cast<float>(10000.0 / 60.0 / 60.0 / 60.0);

	static constexpr float Scale		= 0.002f;	// スケール
	static constexpr float Gravity		= 0.3f;     // キャラに掛かる重力加速度
	static constexpr float JumpPower	= 9.0f;     // キャラのジャンプ力
};
