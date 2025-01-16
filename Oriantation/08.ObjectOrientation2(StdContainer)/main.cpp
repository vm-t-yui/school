//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Common.h"
#include "Player.h"
#include "Shot.h"
#include "Enemy.h"
//#include <vector>
#include <list>

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------

//----------------------------//
// グローバル変数.
//----------------------------//
Player player;
Enemy enemy;
//Shot shot[SHOT];
std::list<Shot> shot(SHOT);

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
	enemy.Init();

	// vector型だったり、中身が増減しないならそのまま使える
	//for (int i = 0; i < SHOT; i++)
	//{
	//	shot[i].Init();
	//}
	// 配列と同じ添え字アクセスが出来ないやつ
	// 中身が増減するやつはこっち
	// 範囲for分
	for (auto& item : shot)
	{
		item.Init();
	}

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		// 各アップデート関数を呼ぶ.
		player.Update(shot);
		enemy.Update();

		// Xキーを押したらショットの中身が増える
		if (CheckHitKey(KEY_INPUT_X) == 1)
		{
			// !この方法だと、新しく作った初期化関数Initが呼べない！
			//shot.emplace_back();	// 後ろに追加

			// !この方法だとInitは呼べるものの、
			// 無駄なローカル変数が生まれて破棄される！
			Shot newShot;
			newShot.Init();
			shot.emplace_back(newShot);

			// ほんとはポインタにしておくと無駄が少ない
		}
		printfDx("弾の総数%d\n", shot.size() );
		for (auto& item : shot)
		{
			item.Update(enemy);
		}

		// 各描画関数を呼ぶ.
		for (auto& item : shot)
		{
			item.Draw();
		}
		player.Draw();
		enemy.Draw();

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