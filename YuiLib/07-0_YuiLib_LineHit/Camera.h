#pragma once

/// <summary>
/// カメラクラス
/// </summary>
class Camera
{
public:
	Camera();
	void Update();

private:
	float	fov;
	float	fovRad;
	VECTOR	pos;
	float	posRad;	// pos変化用の

	constexpr static float DefaultFov		= 60.0f;	// デフォルトのfov
	constexpr static float FovRange			= 30.0f;	// fovの変化幅
	constexpr static float FovChangeSpeed	= 0.01f;	// fovの変化スピード
	constexpr static float PosRange			= 3.0f;		// posの変化幅
	constexpr static float PosChangeSpeed	= 0.03f;	// posの変化スピード
};

