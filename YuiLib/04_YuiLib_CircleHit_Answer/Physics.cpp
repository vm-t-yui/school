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

		// もともとの情報をデバッグ表示
		// FIXME:見たい情報は当たり判定だが、補正後か補正前かわかりづらい
#if _DEBUG
		DebugDraw::DrawLine(pos, nextPos, 0xff00ff);
		DebugDraw::DrawCircle(pos, item->radius, 0xff0000);
#endif

		// ポジション確定
		item->rigidbody.SetPos(nextPos);
	}

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
					// collidable同士、円と円の当たり判定を行う
					auto atob = VSub(objB->rigidbody.GetPos(),
						objA->rigidbody.GetPos());
					auto atobLength = VSize(atob);

					// お互いの距離が、それぞれの半径を足したものより小さければ当たる
					if (atobLength < objA->radius + objB->radius)
					{
						// 衝突によるポジション補正
						auto tagA = objA->GetTag();
						auto tagB = objB->GetTag();

						// 片方がプレイヤーならエネミーのほうを移動
						Collidable* player = nullptr;
						Collidable* enemy = nullptr;
						if (tagA == Collidable::Tag::Player
							&& tagB == Collidable::Tag::Enemy)
						{
							player = objA;
							enemy = objB;
						}
						else if (tagB == Collidable::Tag::Player
							&& tagA == Collidable::Tag::Enemy)
						{
							player = objB;
							enemy = objA;
						}
						if(player && enemy)
						{
							VECTOR playerToEnemy = VSub(enemy->rigidbody.GetPos(),
								player->rigidbody.GetPos());
							VECTOR playerToEnemyN = VNorm(playerToEnemy);

							// HACK: 先に位置を補正してしまうと、過去が参照できない
							VECTOR oldEnemyPos = enemy->rigidbody.GetPos();
							VECTOR playerToNewEnemyPos = VScale(playerToEnemyN, (player->radius + enemy->radius));
							VECTOR fixedPos = VAdd(player->rigidbody.GetPos(), playerToNewEnemyPos);
							enemy->rigidbody.SetPos(fixedPos);

							// HACK: playerもenemyも何回も呼ばれる可能性はある
							// 衝突通知
							player->OnCollide(*enemy);
							enemy->OnCollide(*player);

							// 補正後の位置をデバッグ表示
#if _DEBUG
							DebugDraw::DrawLine(oldEnemyPos, enemy->rigidbody.GetPos(), 0x0000ff);
#endif
							// 一度でもヒット+補正したら衝突判定と補正やりなおし
							doCheck = true;
						}
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
			printfDx("当たり判定の繰り返しチェックが規定数を超えた\n");
			break;
		}
	}
}