// 2016 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// カメラ
/// </summary>
class Camera
{
public:
	Camera();							// コンストラクタ
	~Camera();							// デストラクタ

	// 2D→3D変換の時に歪みを最小限にするため、視野角を狭める
	constexpr static float FovDegree	= 10.0f;	// 視野角
	//constexpr static float FovDegree	= 60.0f;	// 視野角

	// near,farはなるべく小さくする
	constexpr static float CameraNear	= 50.0f;	// near
	constexpr static float CameraFar	= 100.0f;	// far
	//constexpr static float CameraNear	= 0.1f;		// near
	//constexpr static float CameraFar	= 1000.0f;	// far

	// 視野角を狭めたら、表示範囲が狭い＝カメラがズームしたような見た目になるので
	// Z位置を下げる
	constexpr static float CameraZPos	= -51.5f;	// Z位置
	//constexpr static float CameraZPos	= -7.8f;	// Z位置


#if _DEBUG
	void DrawDebug();					// デバッグ描画
#endif // _DEBUG
};
