//-----------------------------------------------------------------------------
// 2024 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
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
	VECTOR	pos;		// 座標 HACK: プレイヤーの座標の中心は、横：中心　縦：中心。描画や計算を変えたら全部変わるので注意
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
struct MapChip;
struct Camera;
void InitPlayer(Player& player);
void UpdatePlayer(Player& player, const Map& map);
VECTOR CheckPlayerHitWithMap(Player& player, const Map& map, const VECTOR& velocity);
bool IsHitPlayerWithMapChip(const Player& player, const VECTOR& futurePos, const MapChip& mapChip);
void CheckIsTopHit(Player& player, const Map& map);
void CheckIsGround(Player& player, const Map& map);
void DrawPlayer(const Player& player, const Camera& camera);
