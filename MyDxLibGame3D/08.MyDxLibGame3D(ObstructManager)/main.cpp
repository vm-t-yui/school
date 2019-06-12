//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include "ObstructStatic.h"
#include "ObstructFloat.h"
#include "ObstructBase.h"

// 障害物の数.
#define OBSTRUCT_NUM 3

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		return -1;	// エラーが起きたら直ちに終了
	}

	// 画面モードのセット
	SetGraphMode(640, 480, 16);
	ChangeWindowMode(TRUE);

	// カメラを生成.
	Camera* camera = new Camera();

	// プレイヤーを生成.
	Player* player = new Player();

	// 障害物を生成.
    ObstructBase* obstructs[OBSTRUCT_NUM];

    obstructs[0] = new ObstructStatic();
    obstructs[1] = new ObstructFloat();
    obstructs[2] = new ObstructStatic();

    // 障害物の位置を初期化.
    float band = 10.0f;
    for (int i = 0; i < OBSTRUCT_NUM; ++i)
    {
        obstructs[i]->SetPos(VGet(-band + (band * i), 0, -1.0f));
    }

	// エスケープキーが押されるかウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// プレイヤー制御.
		player->Update();

		// カメラ制御.
		camera->Update(*player);

		// 障害物制御.
        for (int i = 0; i < OBSTRUCT_NUM; ++i)
        {
            if (obstructs[i] != NULL)
            {
                obstructs[i]->Update();
            }
        }

		// 画面を初期化する
		ClearDrawScreen();

		// プレイヤー描画.
		player->Draw();

		// 障害物描画.
        for (int i = 0; i < OBSTRUCT_NUM; ++i)
        {
            if (obstructs[i] != NULL)
            {
                obstructs[i]->Draw();
            }
        }

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	// プレイヤーを削除.
	delete(player);

	// カメラを削除.
	delete(camera);

	// 障害物を削除.
    for (int i = 0; i < OBSTRUCT_NUM; ++i)
    {
        delete(obstructs[i]);
        obstructs[i] = NULL;
    }

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}