//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#pragma once

class ObstructManager;
class Player;

class HitChecker
{
public:
	static void Check(Player& player, ObstructManager& obstructManager);
};