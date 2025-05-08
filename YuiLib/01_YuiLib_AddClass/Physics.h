﻿// 2024 Takeru Yui All Rights Reserved.
#pragma once

namespace YuiLib{

class Collidable;

/// <summary>
/// 物理をつかさどる
/// </summary>
class Physics final
{
public:
	// 衝突物の登録・登録解除
	void Entry(Collidable* collidable);
	void Exit(Collidable* collidable);

	void Update();	// 更新
};

}
