#pragma once

// 当たったものの判別を行うためのタグ
enum class GameObjectTag
{
	Player,			// プレイヤー
	Enemy,			// 敵
	Axe,			// 斧
	SystemWall,		// システム壁
	StepGround,		// 足場の地面
};