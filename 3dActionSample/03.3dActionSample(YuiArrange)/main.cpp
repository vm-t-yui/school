// ３Ｄアクション基本
// 2023 Takeru Yui All Rights Reserved.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!注意!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//ステージモデルとの当たり判定処理は、下記の前提がある
//前提を無視すると、壁や床のすり抜けや嵌まり現象が発生する
//・壁は床に対して完全に垂直(少しでも垂直でないと「坂」として処理)
//・壁ポリゴンが、水平方向の差があまりない状態で重なっているとめり込む
//・床ポリゴンが、Ｙ軸方向の差があまりない状態で重なっているとめり込む
//・９０度以下の鋭角なカドをがあると、外側から当たっても内側から当たっても、すり抜けや嵌りが発生する
//・「ステージモデル」は、巨大な一つのモデルファイルである
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!注意!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include "DxLib.h"
#include "Camera.h"
#include "Input.h"
#include "Stage.h"
#include "Player.h"

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(TRUE);

	// 初期化
	if (DxLib_Init() < 0)
	{
		return -1;
	}

	Input*	input = new Input();	// 入力情報
	Player*	player = new Player();	// プレイヤー
	Stage*	stage = new Stage();	// ステージ
	Camera*	camera = new Camera();	// カメラ

	player->Load();
	stage->Load();

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// ＥＳＣキーが押されるか、ウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間

		// 画面をクリア
		ClearDrawScreen();

		input->Update();
		player->Update(*input, *camera, *stage);
		camera->Update(*input, *player, *stage);
		
		stage->Draw();
		player->Draw(*stage);

		// 裏画面の内容を表画面に反映
		ScreenFlip();

		// 雑なfps固定処理
		// 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
		auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
		while (afterTime - prevTime < 16667)
		{
			afterTime = GetNowHiPerformanceCount();
			Sleep(1);
		}
	}

	// 後始末
	player->Unload();
	stage->Unload();
	delete(input);
	delete(player);
	delete(stage);
	delete(camera);
	DxLib_End();

	// ソフト終了
	return 0;
}
