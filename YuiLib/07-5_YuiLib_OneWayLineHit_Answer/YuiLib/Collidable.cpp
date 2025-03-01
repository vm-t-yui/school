﻿// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "YuiLib.h"
#include <cassert> 

using namespace YuiLib;

/// <summary>
/// コンストラクタ
/// </summary>
Collidable::Collidable(Priority priority, GameObjectTag tag, ColliderData::Kind colliderKind)
	: priority		(priority)
	, tag			(tag)
	, colliderData	(nullptr)
	, nextPos		(VGet(0,0,0))
{
	CreateColliderData(colliderKind);
}

/// <summary>
/// デストラクタ
/// </summary>
Collidable::~Collidable()
{
	if (colliderData != nullptr)
	{
		delete colliderData;
		colliderData = nullptr;
	}
}

/// <summary>
/// 初期化
/// </summary>
void Collidable::Initialize(YuiLib::Physics* physics)
{
	physics->Entry(this);	// 物理情報に自身を登録
}

/// <summary>
/// 終了
/// </summary>
void Collidable::Finalize(YuiLib::Physics* physics)
{
	physics->Exit(this);	// 物理情報登録解除
}

/// <summary>
/// 当たり判定データの作成
/// </summary>
ColliderData* Collidable::CreateColliderData(ColliderData::Kind kind)
{
	if (colliderData != nullptr)
	{
		assert(0 && "colliderDataは既に作られています。");
		return colliderData;
	}
	switch (kind)
	{
	case ColliderData::Kind::Circle2D:
		colliderData = new ColliderDataCircle2D();
		break;
	case ColliderData::Kind::Line2D :
		colliderData = new ColliderDataLine2D();
		break;
	case ColliderData::Kind::OneWayLine2D:
		colliderData = new ColliderDataOneWayLine2D();
		break;
	}
	return colliderData;
}