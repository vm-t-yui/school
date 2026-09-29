// ３Ｄアクション基本
// 
//＜注意＞
//このサンプルプログラムで行っているステージモデルとの当たり判定処理はステージモデルが以下の項目を考慮して
//作成されていることを前提としています。
//この項目を考慮していないステージモデルでは壁や床のすり抜けや嵌まり現象が発生しますので、注意してください。
//・壁としたい面は床に対して完全に垂直にしてください(少しでも垂直ではないと「坂」として処理されます)
//・壁ポリゴンが水平方向の差があまりない状態で重なっているとめり込むことがあります
//・床ポリゴンがＹ軸方向の差があまりない状態で重なっているとめり込むことがあります
//・９０度以下の鋭角なカドをがあると、外側から当たっても内側から当たっても、すり抜けや嵌りが発生することがあります

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
		return -1;

	Input		input;			// 入力情報
	Player		player;			// プレイヤー
	Stage		stage;			// ステージ
	Camera		camera;			// カメラ

	player.Initialize();
	stage.Initialize();
	camera.Initialize();

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// ＥＳＣキーが押されるか、ウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面をクリア
		ClearDrawScreen();

		input.Update();
		player.Update(input, camera, stage);
		camera.Update(input, player, stage);
		
		stage.Draw();
		player.Draw(stage);

		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	// 後始末
	player.Finalize();
	stage.Finalie();
	DxLib_End();

	// ソフト終了
	return 0;
}
