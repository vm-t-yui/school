//-----------------------------------------------------------------------------
// @brief  ゲームのメインクラス.
// Copyright (c) 2017 VikingMaxx Takeru Yui. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef _GAME_H_
#define _GAME_H_

class VMMesh;
class Camera;

class Game final
{
public:
    Game();					// コンストラクタ.
    ~Game();				// デストラクタ.

    void Initialize();		// 初期化.
    void Finalize();		// 終了処理.
    void Update();			// アップデート.
    void Render();			// 描画.

private:
    VMMesh* mesh;
    Camera* camera;
};

#endif // _GAME_H_