// 2024 Takeru Yui All Rights Reserved.
#pragma once

#include <list>
#include <vector>

namespace YuiLib{

class Collidable;

/// <summary>
/// 物理・衝突判定するオブジェクトを登録し、物理移動・衝突を通知する
/// </summary>
class Physics final
{
public:
	// 衝突物の登録・登録解除
	void Entry(Collidable* collidable);
	void Exit(Collidable* collidable);

	void Update();	// 更新（登録オブジェクトの物理移動、衝突通知）

private:
	std::list<Collidable*> collidables;	// 登録されたCollidableのリスト

	// OnCollideの遅延通知のためのデータ
	struct OnCollideInfo
	{
		Collidable* owner;
		Collidable* colider;
		void OnCollide() { owner->OnCollide(*colider); }
	};

	// 当たり判定チェック
	void CheckColide(std::vector<OnCollideInfo>& onCollideInfo);

	// 次位置補正
	void FixNextPosition(std::vector<OnCollideInfo>& onCollideInfo, Collidable* objA, Collidable* objB);
	void FixPosition();	// 位置の確定
};

}
