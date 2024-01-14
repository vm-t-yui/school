//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
// ゲームオブジェクト
#pragma once
#include "DxLib.h"

struct GameObject
{
	int		graph = -1;
	VECTOR	pos = VGet(0, 0, 0);
	VECTOR	dir = VGet(0, 1, 0);
	float	speed = 0.0f;
	int		w = 0;
	int		h = 0;
	float	angle = 0;
	float	scale = 1.0f;
};

// ゲームオブジェクト移動
void MoveGameObject(GameObject& obj);

// ゲームオブジェクト描画。違う画像も描画できるようにgraphだけ分けている
void DrawGameObject(GameObject& obj, int graph);

// 当たり判定
bool IsHit(GameObject& target1, GameObject& target2);