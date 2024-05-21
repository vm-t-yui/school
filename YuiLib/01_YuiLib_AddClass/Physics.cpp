// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "Physics.h"

using namespace YuiLib;

/// <summary>
/// 衝突物の登録
/// </summary>
void Physics::Entry(const Collidable* collidable)
{
	// TODO:登録
	printfDx("登録！");
}

/// <summary>
/// 衝突物の登録解除
/// </summary>
void Physics::Exit(const Collidable* collidable)
{
	// TODO:登録
	printfDx("登録解除！");
}

/// <summary>
/// 更新
/// </summary>
void Physics::Update()
{
	// TODO:衝突情報の更新と通知
	printfDx("更新！");
}