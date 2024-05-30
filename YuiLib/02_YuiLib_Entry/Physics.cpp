// 2024 Takeru Yui All Rights Reserved.
#include <cassert>
#include "DxLib.h"
#include "Rigidbody.h"
#include "Collidable.h"
#include "Physics.h"

using namespace YuiLib;

/// <summary>
/// 衝突物の登録
/// </summary>
void Physics::Entry(Collidable* collidable)
{
	// 登録
	bool found = (std::find(collidables.begin(), collidables.end(), collidable) != collidables.end());
	if(!found)
	{
		collidables.emplace_back(collidable);
		//collidables.push_back(collidable);
	}
	// 既に登録されてたらエラー
	else
	{
		assert(0 && "指定のcollidableは登録済です。");
	}
}

/// <summary>
/// 衝突物の登録解除
/// </summary>
void Physics::Exit(Collidable* collidable)
{
	//// 登録解除(erace)
	//for (std::list<Collidable*>::const_iterator iterator = collidables.begin();
	//	iterator != collidables.end();
	//	++iterator)
	//{
	//	if ((*iterator) == collidable)
	//	{
	//		collidables.erase(iterator);
	//		break;
	//	}
	//}
	// 登録解除(remove)
	bool found = (std::find(collidables.begin(), collidables.end(), collidable) != collidables.end());
	if (found)
	{
		collidables.remove(collidable);
	}
	// 登録されてなかったらエラー
	else
	{
		assert(0 && "指定のcollidableが登録されていません。");
	}
}

/// <summary>
/// 更新（登録オブジェクトの物理移動、衝突通知）
/// </summary>
void Physics::Update()
{
	// TODO:衝突情報の更新と通知
	for (auto& item : collidables)
	{
		//if(衝突したら)
		item->OnCollide();
	}
}