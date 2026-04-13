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

	// シャドウマップハンドルの作成
	auto ShadowMapHandle = MakeShadowMap(2048, 2048);
	
	// ライトの方向を設定
	auto lightDirection = VGet(0.5f, -0.5f, 0.5f);
	SetLightDirection(lightDirection);

	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(ShadowMapHandle, lightDirection);

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(ShadowMapHandle, VGet(-3000.0f, -1.0f, -3000.0f), VGet(3000.0f, 1000.0f, 3000.0f));

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

		// シャドウマップへの描画の準備
		ShadowMap_DrawSetup(ShadowMapHandle);
		{
			// まずはシャドウマップにステージとキャラクターを描画
			stage->Draw();
			player->Draw();
		}
		ShadowMap_DrawEnd();	// シャドウマップへの描画を終了

		// 通常描画
		SetUseShadowMap(0, ShadowMapHandle);	// 先ほど描画したシャドウマップを使って、普通の描画を行う
		{
			stage->Draw();
			player->Draw();
		}
		SetUseShadowMap(0, -1);	// 描画に使用するシャドウマップの設定を解除

		// 裏画面の内容を表画面に反映
		ScreenFlip();

		// 雑なfps固定処理
		// 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
		auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
		while (afterTime - prevTime < 16667)
		{
			afterTime = GetNowHiPerformanceCount();
		}
	}

	// 後始末
	player->Unload();
	stage->Unload();
	delete(input);
	delete(player);
	delete(stage);
	delete(camera);
	
	DeleteShadowMap(ShadowMapHandle);	// シャドウマップの削除

	DxLib_End();

	// ソフト終了
	return 0;
}
