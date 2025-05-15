// 2024 Takeru Yui All Rights Reserved.
#include <cassert>
#include "DxLib.h"
#include "YuiLib.h"

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
	//// 登録解除(remove + erace)
	//bool found = (std::find(collidables.begin(), collidables.end(), collidable) != collidables.end());
	//if (found)
	//{
	//	collidables.erase(std::remove(collidables.begin(), collidables.end(), collidable), collidables.end());
	//}
	//// 登録されてなかったらエラー
	//else
	//{
	//	assert(0 && "指定のcollidableが登録されていません。");
	//}
	// 登録解除(eraseif 要C++20)
	auto count = std::erase_if(collidables, [collidable](Collidable* target) { return target == collidable; });
	// 登録されてなかったらエラー
	if (count <= 0)
	{
		assert(0 && "指定のcollidableが登録されていません。");
	}
}

/// <summary>
/// 更新（登録オブジェクトの物理移動、衝突通知）
/// </summary>
void Physics::Update()
{
	// 衝突情報の更新と通知
	for (auto& item : collidables)
	{
		// TODO:実装
		// ポジションに移動力を足す

		// デバッグ表示
#if _DEBUG
		//DebugDraw::DrawLine(pos, nextPos, 0xff00ff);
#endif

		// 移動

		//if(衝突したら)
		{
			// 衝突によるポジション補正
			// TODO:実装

			// 衝突通知
			//item->OnCollide();
		}
	}
}