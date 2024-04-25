// 2024 Takeru Yui All Rights Reserved.
#pragma once

namespace YuiLib {

class Rigidbody;

/// <summary>
/// Õ“Ë‚Å‚«‚é‚à‚Ì
/// </summary>
class Collidable
{
public:
	void OnCollide();	// Õ“Ë‚µ‚½‚Æ‚«

private:
	Rigidbody rigidbody;
};

}

