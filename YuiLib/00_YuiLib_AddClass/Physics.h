// 2024 Takeru Yui All Rights Reserved.
#pragma once

namespace YuiLib{

class Collidable;

/// <summary>
/// •¨—‚ğ‚Â‚©‚³‚Ç‚é
/// </summary>
class Physics final
{
public:
	// Õ“Ë•¨‚Ì“o˜^E“o˜^‰ğœ
	void Entry(const Collidable* collidable);
	void Exit(const Collidable* collidable);

	void Update();	// XV
};

}
