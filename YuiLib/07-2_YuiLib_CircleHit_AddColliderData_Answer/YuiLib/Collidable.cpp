// 2024 Takeru Yui All Rights Reserved.
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
	}
	return colliderData;
}