﻿// 2024 Takeru Yui All Rights Reserved.
#pragma once

#include <list>

namespace YuiLib{

class Collidable;

/// <summary>
/// 物理・衝突判定するオブジェクトを登録し、物理移動・衝突を通知する
/// </summary>
class Physics final
{
public:
	// 衝突物の登録・登録解除
	void Entry(Collidable* collidable);
	void Exit(Collidable* collidable);

	void Update();	// 更新（登録オブジェクトの物理移動、衝突通知）

private:
	std::list<Collidable*> collidables;	// 登録されたCollidableのリスト
};

}
