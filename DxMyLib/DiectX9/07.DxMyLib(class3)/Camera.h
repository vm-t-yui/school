//-----------------------------------------------------------------------------
// @brief  カメラクラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Device.h"

class Camera final
{
public:
	Camera();				        // コンストラクタ.
	~Camera();				        // デストラクタ.

	void Initialize();				// 初期化.
	void Finalize();                // 終了処理.
	void Update();

	void SetCameraMatrices(const Device& device);
};

#endif // _CAMERA_H_