﻿// 2024 Takeru Yui All Rights Reserved.
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
	// 移動
	for (auto& item : collidables)
	{
		// ポジションに移動力を足す
		auto pos = item->rigidbody.GetPos();
		auto nextPos = VAdd(pos, item->rigidbody.GetVelocity());

		// もともとの情報、予定情報をデバッグ表示
#if _DEBUG
		if (item->colliderData->GetKind() == ColliderData::Kind::Circle2D)
		{
			ColliderDataCircle2D* circleData;
			circleData = static_cast<ColliderDataCircle2D*>(item->colliderData);
			float radius = circleData->radius;
			DebugDraw::DrawCircle(pos, radius, BeforeFixInfoColor);
			DebugDraw::DrawLine(pos, nextPos, AimInfoColor);
			DebugDraw::DrawCircle(nextPos, radius, AimInfoColor);
		}
#endif

		// 予定ポジション設定
		item->nextPos = nextPos;
	}

	// 当たり判定チェック（nextPos指定）
	std::vector<OnCollideInfo> onCollideInfo = CheckColide();

	// 位置確定
	FixPosition();

	// 当たり通知
	for (auto& item : onCollideInfo)
	{
		item.owner->OnCollide(*item.colider);
	}
}

/// <summary>
/// 当たり判定チェック
/// </summary>
std::vector<Physics::OnCollideInfo> Physics::CheckColide() const
{
	std::vector<OnCollideInfo> onCollideInfo;
	// 衝突通知、ポジション補正
	bool	doCheck = true;
	int		checkCount = 0;	// チェック回数
	while (doCheck)
	{
		doCheck = false;
		++checkCount;

		// 2重ループで全オブジェクト当たり判定
		// FIXME: 重いので近いオブジェクト同士のみ当たり判定するなど工夫がいる
		for (auto& objA : collidables)
		{
			for (auto& objB : collidables)
			{
				if (objA != objB)
				{
					// お互いの距離が、それぞれの半径を足したものより小さければ当たる
					if (IsCollide(objA, objB))
					{
						// 次目標位置修正
						auto priorityA = objA->GetPriority();
						auto priorityB = objB->GetPriority();

						// プライオリティの高いほうを移動
						Collidable* primary = objA;
						Collidable* secondary = objB;
						if (priorityA < priorityB)
						{
							primary = objB;
							secondary = objA;
						}
						FixNextPosition(primary, secondary);

						// 衝突通知情報の更新
						// HACK: primaryもsecondaryも何回も呼ばれる可能性はあるので、排他遅延処理
						bool hasPrimaryInfo = false;
						bool hasSecondaryInfo = false;
						for (const auto& item : onCollideInfo)
						{
							// 既に通知リストに含まれていたら呼ばない
							if (item.owner == primary)
							{
								hasPrimaryInfo = true;
							}
							if (item.owner == secondary)
							{
								hasSecondaryInfo = true;
							}
						}
						if (!hasPrimaryInfo)
						{
							onCollideInfo.push_back({ primary, secondary });
						}
						if (!hasSecondaryInfo)
						{
							onCollideInfo.push_back({ secondary, primary });
						}

						// 一度でもヒット+補正したら衝突判定と補正やりなおし
						doCheck = true;
						break;
					}
				}
			}
			if (doCheck)
			{
				break;
			}
		}

		// 無限ループ避け
		// FIXME: 定数化
		if (checkCount > 1000 && doCheck)
		{
#if _DEBUG
			printfDx("当たり判定の繰り返しチェックが規定数を超えた\n");
#endif
			break;
		}
	}
	return onCollideInfo;
}

/// <summary>
/// 当たっているかどうかだけ判定
/// </summary>
bool Physics::IsCollide(const Collidable* objA, const Collidable* objB) const
{
	// TODO: ラインと円、円と円で当たり判定を分ける。壁はライン
	// TODO: collidableの種類によって、当たり判定を分ける
	// TODO: 当たり判定の種別を準備し、それによって計算を分ける
	auto atob = VSub(objB->nextPos, objA->nextPos);
	auto atobLength = VSize(atob);

	// お互いの距離が、それぞれの半径を足したものより小さければ当たる
	auto objAColliderData = static_cast<ColliderDataCircle2D*>(objA->colliderData);
	auto objBColliderData = static_cast<ColliderDataCircle2D*>(objB->colliderData);
	return (atobLength < objAColliderData->radius + objBColliderData->radius);
}

/// <summary>
/// 次位置補正
/// </summary>
void Physics::FixNextPosition(Collidable* primary, Collidable* secondary) const
{
	// TODO: 当たり判定の種別を準備し、それによって補正位置を変える
	VECTOR primaryToSecondary = VSub(secondary->nextPos, primary->nextPos);
	VECTOR primaryToSecondaryN = VNorm(primaryToSecondary);

	auto primaryColliderData = static_cast<ColliderDataCircle2D*>(primary->colliderData);
	auto secondaryColliderData = static_cast<ColliderDataCircle2D*>(secondary->colliderData);
	float  awayDist = primaryColliderData->radius + secondaryColliderData->radius + 0.0001f;	// そのままだとちょうど当たる位置になるので少し余分に離す
	VECTOR primaryToNewSecondaryPos = VScale(primaryToSecondaryN, awayDist);
	VECTOR fixedPos = VAdd(primary->nextPos, primaryToNewSecondaryPos);
	secondary->nextPos = fixedPos;
}

/// <summary>
/// 位置確定
/// </summary>
void Physics::FixPosition()
{
	for (auto& item : collidables)
	{
#if _DEBUG
		// 補正後の位置をデバッグ表示
		DebugDraw::DrawLine(item->rigidbody.GetPos(), item->nextPos, AfterFixInfoColor);

		if (item->colliderData->GetKind() == ColliderData::Kind::Circle2D)
		{
			auto itemCircleData = static_cast<ColliderDataCircle2D*>(item->colliderData);
			DebugDraw::DrawCircle(item->nextPos, itemCircleData->radius, AfterFixInfoColor);
		}
#endif

		// 位置確定
		item->rigidbody.SetPos(item->nextPos);
	}
}