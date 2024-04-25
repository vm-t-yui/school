// 2024 Takeru Yui All Rights Reserved.
#pragma once

namespace YuiLib {

class Rigidbody;

/// <summary>
/// インターフェース：衝突できるもの
/// </summary>
class Collidable abstract
{
public:
	virtual void OnCollide() abstract;	// 衝突したとき

private:
	Rigidbody rigidbody;
};

}

