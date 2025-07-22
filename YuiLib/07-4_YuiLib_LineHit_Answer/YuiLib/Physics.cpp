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
	if (!found)
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
			circleData = dynamic_cast<ColliderDataCircle2D*>(item->colliderData);
			float radius = circleData->radius;
			DebugDraw::DrawCircle(pos, radius, BeforeFixInfoColor);
			DebugDraw::DrawLine(pos, nextPos, AimInfoColor);
			DebugDraw::DrawCircle(nextPos, radius, AimInfoColor);
		}
		if (item->colliderData->GetKind() == ColliderData::Kind::Line2D)
		{
			// lineはstaticなので確定色で描く
			ColliderDataLine2D* lineData;
			lineData = dynamic_cast<ColliderDataLine2D*>(item->colliderData);
			DebugDraw::DrawLine(lineData->startPoint, lineData->endPoint, AfterFixInfoColor);
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
	bool isHit = false;

	// collidableの種類によって、当たり判定を分ける
	auto aKind = objA->colliderData->GetKind();
	auto bKind = objB->colliderData->GetKind();

	// ラインと円、円と円で当たり判定を分ける
	if (aKind == ColliderData::Kind::Circle2D && bKind == ColliderData::Kind::Circle2D)
	{
		auto atob = VSub(objB->nextPos, objA->nextPos);
		auto atobLength = VSize(atob);

		// お互いの距離が、それぞれの半径を足したものより小さければ当たる
		auto objAColliderData = dynamic_cast<ColliderDataCircle2D*>(objA->colliderData);
		auto objBColliderData = dynamic_cast<ColliderDataCircle2D*>(objB->colliderData);
		isHit = (atobLength < objAColliderData->radius + objBColliderData->radius);
	}
	else if (
		(aKind == ColliderData::Kind::Line2D && bKind == ColliderData::Kind::Circle2D)
		 ||
		(aKind == ColliderData::Kind::Circle2D && bKind == ColliderData::Kind::Line2D)
		)
	{
		ColliderData* lineDataBase = objA->colliderData;
		ColliderData* circleDataBase = objB->colliderData;
		VECTOR circleCenter = objB->nextPos;
		if (bKind == ColliderData::Kind::Line2D)
		{
			lineDataBase = objB->colliderData;
			circleDataBase = objA->colliderData;
			circleCenter = objA->nextPos;
		}
		auto lineColliderData = dynamic_cast<ColliderDataLine2D*>(lineDataBase);
		auto circleColliderData = dynamic_cast<ColliderDataCircle2D*>(circleDataBase);
		isHit = (Segment_Point_MinLength(lineColliderData->startPoint, lineColliderData->endPoint, circleCenter) < circleColliderData->radius);
	}

	return isHit;
}

/// <summary>
/// 線分に対する最近接点を求める
/// </summary>
VECTOR GetClosestPtOnSegment(VECTOR pt, VECTOR start, VECTOR end)
{
	// 最近接点がstart側線分外領域の場合
	VECTOR startToPt = VSub(pt, start);
	VECTOR startToEnd = VSub(end, start);
	VECTOR startToEndN = VNorm(startToEnd);
	if (VDot(startToPt, startToEndN) < 0)
	{
		return start;
	}
	VECTOR endToPt = VSub(pt, end);
	VECTOR endToStart = VSub(start, end);
	VECTOR endToStartN = VNorm(endToStart);
	// 最近接点がend側線分外領域の場合
	if (VDot(endToPt, endToStartN) < 0)
	{
		return end;
	}
	// 中間領域の場合
	else
	{
		float t = VDot(startToEndN, startToPt);
		return VAdd(start, VScale(startToEndN, t));
	}
}

/// <summary>
/// 次位置補正
/// </summary>
void Physics::FixNextPosition(Collidable* primary, Collidable* secondary) const
{
	// 当たり判定の種別ごとに補正方法を変える
	auto primaryKind = primary->colliderData->GetKind();
	auto secondaryKind = secondary->colliderData->GetKind();

	// 円同士の位置補正
	if (primaryKind == ColliderData::Kind::Circle2D && secondaryKind == ColliderData::Kind::Circle2D)
	{
		VECTOR primaryToSecondary = VSub(secondary->nextPos, primary->nextPos);
		VECTOR primaryToSecondaryN = VNorm(primaryToSecondary);

		auto primaryColliderData = dynamic_cast<ColliderDataCircle2D*>(primary->colliderData);
		auto secondaryColliderData = dynamic_cast<ColliderDataCircle2D*>(secondary->colliderData);
		float  awayDist = primaryColliderData->radius + secondaryColliderData->radius + 0.0001f;	// そのままだとちょうど当たる位置になるので少し余分に離す
		VECTOR primaryToNewSecondaryPos = VScale(primaryToSecondaryN, awayDist);
		VECTOR fixedPos = VAdd(primary->nextPos, primaryToNewSecondaryPos);
		secondary->nextPos = fixedPos;
	}
	// ラインと円の位置補正
	// HACK:現状lineは必ずstaticなのでprimaryにline,secondaryにcircleがくる。そうでないときはエラーを吐く
	else if (primaryKind == ColliderData::Kind::Line2D && secondaryKind == ColliderData::Kind::Circle2D)
	{
		auto lineData = dynamic_cast<ColliderDataLine2D*>(primary->colliderData);
		auto circleData = dynamic_cast<ColliderDataCircle2D*>(secondary->colliderData);
		// secondaryが移動する円として処理

		// 1.過去の位置→未来の位置の線分と、壁線分が交わるかどうかを調べる
		float bothSegmentLength = (
			Segment_Segment_MinLength(secondary->nextPos, secondary->rigidbody.GetPos(),
				lineData->startPoint, lineData->endPoint)
			);
		bool isCross = (bothSegmentLength == 0);	// ２つの線分の最近接点までの距離が0なら交差している

		// 後の計算で結局「壁線分上の、未来の位置（ベクトルの終端の点）との最近接点」が必要になるので
		// 先に出しておく
		VECTOR closestPtOnWallAndNextPos 
			= GetClosestPtOnSegment(secondary->nextPos, lineData->startPoint, lineData->endPoint);

		// 2.交わる場合
		if (isCross)
		{
			// 未来の位置の点から壁線分に対しておろした垂線との交点を調べる
			// →垂線の交点＝「壁線分上の、未来の位置（ベクトルの終端の点）との最近接点」
			// 最終的な位置は、垂線の交点から「未来の位置→垂線との交点 ベクトル方向」に円の半径分動かした位置
			VECTOR nextToClosest = VSub(closestPtOnWallAndNextPos, secondary->nextPos);
			VECTOR nextToClosestN = VNorm(nextToClosest);
			VECTOR fixedPos = VAdd(closestPtOnWallAndNextPos, VScale(nextToClosestN, circleData->radius + 0.0001f));
			secondary->nextPos = fixedPos;
		}
		// 3.交わらない場合
		else
		{
			// 過去の位置→未来の位置の線分と、壁線分の最近接点を調べる
			// その最近接点から壁線分に対しておろした垂線との交点を調べる
			// →垂線の交点＝「壁線分上の、未来の位置（ベクトルの終端の点）との最近接点」
			// 最終的な位置は、垂線の交点から「垂線の交点→未来の位置 ベクトル方向」に円の半径分動かした位置
			VECTOR closestToNext = VSub(secondary->nextPos, closestPtOnWallAndNextPos);
			VECTOR closestToNextN = VNorm(closestToNext);
			VECTOR fixedPos = VAdd(closestPtOnWallAndNextPos, VScale(closestToNextN, circleData->radius + 0.0001f));
			secondary->nextPos = fixedPos;
		}
	}
	else
	{
		assert(0 && "許可されていない当たり判定の位置補正です");
	}
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
			auto itemCircleData = dynamic_cast<ColliderDataCircle2D*>(item->colliderData);
			DebugDraw::DrawCircle(item->nextPos, itemCircleData->radius, AfterFixInfoColor);
		}
#endif
		// Posを更新するので、velocityもそこに移動するvelocityに修正
		VECTOR toFixedPos = VSub(item->nextPos, item->rigidbody.GetPos());
		item->rigidbody.SetVelocity(toFixedPos);

		// 位置確定
		item->rigidbody.SetPos(item->nextPos);
	}
}