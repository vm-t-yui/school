// 2024 Takeru Yui All Rights Reserved.
#pragma once

namespace YuiLib {

/// <summary>
/// 物理、衝突判定に必要なデータの塊
/// </summary>
class Rigidbody
{
public:
	Rigidbody();
	void Initialize();	// 初期化

	// Getter/Setter
	const VECTOR& GetPos() const { return pos; }
	const VECTOR& GetDir() const { return dir; }
	const VECTOR& GetVelocity() const { return velocity; }

	void SetPos(const VECTOR& set) { pos = set; }
	void SetVelocity(const VECTOR& set);

private:
	VECTOR	pos;
	VECTOR	dir;
	VECTOR	velocity;
};

}
