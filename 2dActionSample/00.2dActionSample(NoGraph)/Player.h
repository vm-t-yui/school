#pragma once

//-----------------------------------------//
// 定数
//-----------------------------------------//
const float Gravity = 0.3f;                         // キャラに掛かる重力加速度
const float JumpPower = 9.0f;                         // キャラのジャンプ力
const float Speed = 5.0f;                         // キャラの移動スピード

/// <summary>
/// プレイヤー構造体
/// </summary>
struct Player
{
	VECTOR	pos;		// 座標
	VECTOR	dir;		// 座標
	float	w, h;		// 幅、高さ
	float	fallSpeed;	// プレイヤーの落下速度。ジャンプ時は反転する
	bool	isGround;	// プレイヤーが接地中か
	bool	isHitTop;	// プレイヤーの頭が天井に当たっているか
};

//-----------------------------------------//
// プロトタイプ宣言
//-----------------------------------------//
struct Map;
void InitPlayer(Player& player);
void UpdatePlayer(Player& player, Map& map);
VECTOR CheckPlayerHitWithMap(Player& player, VECTOR velocity);
void CheckIsTopHit(Player& player, Map& map);
void CheckIsGround(Player& player, Map& map);
void DrawPlayer(Player& player);
