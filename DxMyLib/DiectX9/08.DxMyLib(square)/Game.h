//-----------------------------------------------------------------------------
// @brief  ゲームのメインクラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _GAME_H_
#define _GAME_H_

#include "Device.h"

class Triangle;
class Camera;

class Game final
{
public:
	Game();										// コンストラクタ.
	~Game();									// デストラクタ.

	void Initialize(const Device& device);		// 初期化.
	void Finalize();							// 終了処理.
	void Update();								// アップデート.
	void Render(const Device& device);			// 描画.

private:
	Triangle* triangle;
	Camera* camera;
};

#endif // _GAME_H_