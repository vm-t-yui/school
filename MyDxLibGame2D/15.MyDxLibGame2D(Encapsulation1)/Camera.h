// 2016 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// カメラ
/// </summary>
class Camera
{
public:
	Camera();							// コンストラクタ
	virtual ~Camera();							// デストラクタ

#if _DEBUG
	void DrawDebug();					// デバッグ描画
#endif // _DEBUG
};
