﻿#pragma once

// 当たったものの判別を行うためのタグ
enum class GameObjectTag
{
	Player,		// プレイヤー
	Enemy,		// 敵
	SystemWall,	// システム壁
	Ground,		// 地面
};