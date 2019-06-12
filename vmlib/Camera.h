//-----------------------------------------------------------------------------
// @brief  カメラクラス.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
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

	void SetCameraMatrices();
};

#endif // _CAMERA_H_