﻿// 2024 Takeru Yui All Rights Reserved.
#pragma once

#include "../GameObjectTag.h"

namespace YuiLib {

class Rigidbody;
class Physics;
class ColliderData;

/// <summary>
/// 衝突できるもの。物理・衝突判定をする場合はこれを継承させる
/// </summary>
class Collidable abstract
{
public:

	// 位置補正の優先度の判別に使う
	enum class Priority : int
	{
		Low,		// 低
		Middle,		// 中
		High,		// 高
		Static,		// 動かない（最高）
	};

	Collidable(Priority priority, GameObjectTag tag, ColliderData::Kind colliderKind);	// コンストラクタ
	virtual ~Collidable();													// デストラクタ
	virtual void OnCollide(const Collidable& colider) abstract;				// 衝突したとき

	GameObjectTag	GetTag()		const { return tag; }					// タグ情報
	Priority		GetPriority()	const { return priority; }				// 優先度

protected:
	Rigidbody		rigidbody;		// 物理データ
	ColliderData*	colliderData;	// 当たり判定データ

private:
	ColliderData* CreateColliderData(ColliderData::Kind kind);

	GameObjectTag	tag;
	Priority		priority;

	// PhysicsがCollidableを自由に管理するためにフレンド化
	friend Physics;

	// 以下、フレンドであるPhisicsのみが扱う型や変数
private:
	// FIXME: これはPhysicsが持つ方が望ましい
	VECTOR		nextPos;
};

}

