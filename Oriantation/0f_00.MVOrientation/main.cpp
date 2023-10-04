//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Common.h"
#include "Player.h"
#include "Shot.h"
#include "EnemyParam.h"
#include "EnemyParamModel.h"
#include "EnemyUiHp.h"
#include "EnemyParamUI.h"
#include "Enemy.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------

//----------------------------//
// グローバル変数.
//----------------------------//
Player player;
Enemy enemy;
EnemyParamUI enemyParamUI;
Shot shot[SHOT];

//----------------------------//
// WinMain関数.
//----------------------------//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(SCREEN_W, SCREEN_H, 16);		// 解像度をSCREEN_W*SCREEN_H、colorを16bitに設定.
	ChangeWindowMode(TRUE);			// ウインドウモードに.
	
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// 背景のスクロールをするためにテクスチャモードをラッピングに修正.
	SetTextureAddressMode(DX_TEXADDRESS_WRAP);

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// 各初期化を呼ぶ.
	player.Init();
	enemy.Init(enemyParamUI);
	for (int i = 0; i < SHOT; i++)
	{
		shot[i].Init();
	}

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		// 各アップデート関数を呼ぶ.
		player.Update(shot);
		enemy.Update();
		for (int i = 0; i < SHOT; i++)
		{
			shot[i].Update(enemy, enemyParamUI.GetParam());
		}

		// 各描画関数を呼ぶ.
		for (int i = 0; i < SHOT; i++)
		{
			shot[i].Draw();
		}
		player.Draw();
		enemy.Draw(enemyParamUI.GetParam());
		enemyParamUI.Draw();

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