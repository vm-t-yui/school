// 2024 Takeru Yui All Rights Reserved.
#pragma once

namespace YuiLib {

class Rigidbody;
class Physics;

/// <summary>
/// 衝突できるもの。物理・衝突判定をする場合はこれを継承させる
/// </summary>
class Collidable abstract
{
public:
	virtual void OnCollide() abstract;	// 衝突したとき

protected:
	Rigidbody rigidbody;

	// PhysicsがCollidableを自由に管理するためにフレンド化
	friend Physics;
};

}

