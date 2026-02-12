// 2016 Takeru Yui All Rights Reserved.
#pragma once

/// <summary>
/// カメラ実体
/// </summary>
class Camera final
{
public:
	Camera();					// コンストラクタ
	~Camera();					// デストラクタ

	void Update(const VirtualCameraManager& virtualCameraManager);
};
