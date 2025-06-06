﻿//-----------------------------------------------------------------------------
// @brief  2Dと3Dの交互表示
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Common.h"
#include "Player.h"
#include "Enemy.h"

// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	/*///////////////////////////////////
	・05.MyDxLibGame3D(Camera2)_Answerをみながら、
		12.MyDxLibGame2D(Vector_chase)に３D背景としてドラム缶を表示してみる
		（data / model / obstructStatic / obstructStatic.pmd）

		※余計な部分は移植しないこと！

		1.3Dモデル描画に必要な関数をmain関数に移植して、表示を確かめる
		2.カメラの位置を動かし、ドラム缶を背景としてちょうどいい位置に動かす
		3.描画関数の位置を変えることで、
		・プレイヤーとエネミーの後ろにドラム缶が表示される
		・プレイヤーとエネミーの手前にドラム缶が表示される
		ことを確認する
		---- (option)
		4.カメラ関連をCameraクラスに、ドラム缶をBackGround3Dクラスに移動する
		5.カメラをUpdateで常に決まった周期で動かしてみる
		6.カメラのfovをUpdateで決まった周期で変えてみる

		3まで終わった時、6まで終わった時にチームチャンネルに報告
	///////////////////////////////////*/
	
	// 画面モードの設定
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16);		// 解像度を640*480、colorを16bitに設定.
	ChangeWindowMode(TRUE);			// ウインドウモードに.
	
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// プレイヤー初期化
	Player player;
	InitPlayer(player);

	// エネミー初期化.
	Enemy enemy;
	InitEnemy(enemy);

	//////////////////////
	// ベクトル計算のテスト.
	Vector v1, v2, v3;
	v1.x = 0;
	v1.y = 1;
	v2.x = 2;
	v2.y = 3;
	v3 = AddVector(v1, v2);
	printfDx("x:%f y:%f\n", v3.x, v3.y);
	v3 = SubVector(v1, v2);
	printfDx("x:%f y:%f\n", v3.x, v3.y);
	printfDx("len:%f\n", GetVectorLength(v3));
	v3 = NormalizeVector(v3);
	printfDx("x:%f y:%f\n", v3.x, v3.y);
	//////////////////////

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		UpdatePlayer(player);
		UpdateEnemy(enemy, player);

		DrawPlayer(player);
		DrawEnemy(enemy);

		// 裏画面の内容を表画面にコピーする（描画の確定）.
		ScreenFlip();

		// Windows 特有の面倒な処理をＤＸライブラリにやらせる
		// マイナスの値（エラー値）が返ってきたらループを抜ける
		if (ProcessMessage() < 0)
		{
			break;
		}
		// もしＥＳＣキーが押されていたらループから抜ける
		else if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}