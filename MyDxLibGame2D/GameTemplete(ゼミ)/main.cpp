//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include <string>

// main以外は、すべてヘッダでプロトタイプ宣言を行っている
// mainのみ全てのヘッダをインクルードする。順番に注意！
#include "DxLib.h"
#include "Utility.h"
#include "GameObject.h"
#include "Shot.h"
#include "Player.h"
#include "Enemy.h"
#include "BG.h"
#include "UI.h"
#include "Game.h"

//----------------------------//
// WinMain関数.
//----------------------------//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(SCREEN_W, SCREEN_H, 16);		// 解像度をSCREEN_W*SCREEN_H、colorを16bitに設定.

	/////////////////////////////////
	// ウインドウモードに(コメントアウトしたらフルスクリーン).
	ChangeWindowMode(TRUE);
	/////////////////////////////////

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// 背景のスクロールをするためにテクスチャモードをラッピングに修正.
	SetTextureAddressMode(DX_TEXADDRESS_WRAP);

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// ゲーム構造体
	Game game;
	InitGame(game);

	// ゲームループ.
	while (1)
	{
		auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間

		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();
		
		// ゲームの更新
		UpdateGame(game);
		DrawGame(game);

		// 裏画面の内容を表画面にコピーする（描画の確定）.
		ScreenFlip();

		// 雑なfps固定処理
		// 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
		auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
		while (afterTime - prevTime < 16667)
		{
			afterTime = GetNowHiPerformanceCount();
			Sleep(1);
		}

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