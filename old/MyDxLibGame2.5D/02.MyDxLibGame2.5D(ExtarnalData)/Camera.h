// 2016 Takeru Yui All Rights Reserved.
#pragma once

class Player;

/// <summary>
/// カメラ
/// </summary>
class Camera
{
public:
	Camera();							// コンストラクタ.
	~Camera();							// デストラクタ.

	void Update(const Player& player);	// 更新.

	// ポジションのgetter/setter.
	const VECTOR& GetPos() const { return pos; }

private:
	VECTOR	pos;			// ポジション.
};
