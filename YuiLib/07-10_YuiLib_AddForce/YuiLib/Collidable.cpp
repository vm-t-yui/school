// 2024 Takeru Yui All Rights Reserved.
#include <cassert> 
#include "DxLib.h"
#include "YuiLib.h"

using namespace YuiLib;

/// <summary>
/// コンストラクタ
/// </summary>
Collidable::Collidable(Priority priority, GameObjectTag tag, ColliderData::Kind colliderKind, bool isTrigger)
	: priority		(priority)
	, tag			(tag)
	, colliderData	(nullptr)
	, nextPos		(VGet(0,0,0))
{
	CreateColliderData(colliderKind, isTrigger);
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
/// 当たり判定を無視（スルー）するタグの追加
/// </summary>
void Collidable::AddThroughTag(GameObjectTag tag)
{
	bool found = (std::find(throughTags.begin(), throughTags.end(), tag) != throughTags.end());
	if (found)
	{
		assert(0 && "指定タグは既に追加されています");
	}
	else
	{
		throughTags.emplace_back(tag);
	}
}

/// <summary>
/// 当たり判定を無視（スルー）するタグの削除
/// </summary>
void Collidable::RemoveThroughTag(GameObjectTag tag)
{
	bool found = (std::find(throughTags.begin(), throughTags.end(), tag) != throughTags.end());
	if (!found)
	{
		assert(0 && "指定タグは存在しません");
	}
	else
	{
		throughTags.remove(tag);
	}
}

// 当たり判定を無視（スルー）する対象かどうか
bool Collidable::IsThroughTarget(const Collidable* target) const
{
	bool found = (std::find(throughTags.begin(), throughTags.end(), target->GetTag()) != throughTags.end());
	return found;
}

/// <summary>
/// 当たり判定データの作成
/// </summary>
ColliderData* Collidable::CreateColliderData(ColliderData::Kind kind, bool isTrigger)
{
	if (colliderData != nullptr)
	{
		assert(0 && "colliderDataは既に作られています。");
		return colliderData;
	}
	switch (kind)
	{
	case ColliderData::Kind::Circle2D:
		colliderData = new ColliderDataCircle2D(isTrigger);
		break;
	case ColliderData::Kind::Line2D :
		colliderData = new ColliderDataLine2D(isTrigger);
		break;
	case ColliderData::Kind::OneWayLine2D:
		colliderData = new ColliderDataOneWayLine2D(isTrigger);
		break;
	default:
		assert(0 && "colliderData作成に失敗。");
		break;
	}
	return colliderData;
}