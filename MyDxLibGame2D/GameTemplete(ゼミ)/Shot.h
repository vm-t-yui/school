//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#pragma once

// プロトタイプ宣言
struct GameObject;

// MEMO:ショット自体にGameObject targetを作って一定時間追いかけさせたりとか
struct Shot
{
	GameObject	obj;
	int		power = 0;
	bool	visibleFlag = false;
};

// 初期化.
void InitShot(Shot& shot, int shotGraph, int shotW, int shotH, float angle, float speed, int power);

// ショットの移動.
void MoveShot(Shot& shot);

// ショットの当たり判定チェック
bool IsHitShot(Shot& shot, GameObject& target);

// 描画.
void DrawShot(Shot& shot);