#pragma once

class Input;
class Player;
class Stage;

class Camera
{
public:
	static constexpr float AngleSpeed				= 0.05f	 ;	// 旋回速度
	static constexpr float CameraPlayerTargetHeight	= 400.0f ;	// プレイヤー座標からどれだけ高い位置を注視点とするか
	static constexpr float ToPlayerLength			= 1600.0f;	// プレイヤーとの距離
	static constexpr float CollisionSize			= 50.0f	 ;	// カメラの当たり判定サイズ

	void Initialize();						// カメラの初期化処理
	void Update(const Input& input, const Player& player, const Stage& stage);		// カメラの処理

	const VECTOR& GetEye() const { return Eye; }
	const VECTOR& GetTarget() const { return Target; }

private:
	float		AngleH;					// 水平角度
	float		AngleV;					// 垂直角度
	VECTOR		Eye;					// カメラ座標
	VECTOR		Target;					// 注視点座標

	void FixCameraPosition(const Stage& stage);
};

