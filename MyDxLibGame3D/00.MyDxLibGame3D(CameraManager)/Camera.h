// 2016 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// カメラ実体
/// </summary>
class Camera
{
public:
	Camera();							// コンストラクタ
	virtual ~Camera();					// デストラクタ

private:
	VECTOR pos;
	VECTOR target;
};
