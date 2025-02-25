// 2025 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include "ObstacleKinematic.h"
#include "ObstacleMovable.h"

// 障害物の数
const int ObstacleNum = 3;

/// <summary>
/// メイン関数
/// </summary>
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
    ObstacleKinematic* obstacleKinematic1 = new ObstacleKinematic();
    ObstacleMovable* obstacleMovable = new ObstacleMovable();
    ObstacleKinematic* obstacleKinematic2 = new ObstacleKinematic();

	// 障害物の位置を初期化.
	float band = 10.0f;
    obstacleKinematic1->SetPos(VGet(-band + (band * 0), 0, -1.0f));
    obstacleMovable->SetPos(VGet(-band + (band * 1), 0, -1.0f));
    obstacleKinematic2->SetPos(VGet(-band + (band * 2), 0, -1.0f));

	// エスケープキーが押されるかウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// プレイヤー制御.
		player->Update();

		// カメラ制御.
		camera->Update(*player);

		// 障害物制御.
        obstacleKinematic1->Update();
        obstacleMovable->Update();
        obstacleKinematic2->Update();

		// 画面を初期化する
		ClearDrawScreen();

		// プレイヤー描画.
		player->Draw();

		// 障害物描画.
        obstacleKinematic1->Draw();
        obstacleMovable->Draw();
        obstacleKinematic2->Draw();

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	// プレイヤーを削除.
	delete(player);

	// カメラを削除.
	delete(camera);

	// 障害物を削除.
    delete(obstacleKinematic1);
    delete(obstacleMovable);
    delete(obstacleKinematic2);

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}