//-----------------------------------------------------------------------------
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#pragma once

class Player;	// プロトタイプ宣言

/// <summary>
/// カメラ制御を行う
/// </summary>
class Camera
{
public:
	Camera();							// コンストラクタ
	~Camera();							// デストラクタ

	void Update(const Player& player);	// 更新

private:
	VECTOR	pos;			// ポジション

};