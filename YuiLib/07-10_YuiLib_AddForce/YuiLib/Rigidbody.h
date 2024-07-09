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
	void Initialize(bool useGravity = false);	// 初期化

	// velocityに力を加える
	void AddForce(const VECTOR& force);

	// Getter/Setter
	const VECTOR& GetPos() const { return pos; }
	const VECTOR& GetDir() const { return dir; }
	const VECTOR& GetVelocity() const { return velocity; }
	bool UseGravity() const { return useGravity; }

	void SetPos(const VECTOR& set) { pos = set; }
	void SetVelocity(const VECTOR& set);
	void SetUseGravity(bool set) { useGravity = set; }

private:
	VECTOR	pos;
	VECTOR	dir;
	VECTOR	velocity;
	bool	useGravity;
};

}
