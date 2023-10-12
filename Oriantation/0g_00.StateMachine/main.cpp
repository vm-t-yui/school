//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include <functional>
#include "DxLib.h"
#include "Common.h"
#include "StateMachine.h"
#include "Player.h"
#include "Shot.h"
#include "EnemyParam.h"
#include "EnemyParamModel.h"
#include "EnemyUiHp.h"
#include "EnemyParamUI.h"
#include "Enemy.h"

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

	Player player;
	Enemy enemy;
	EnemyParamUI enemyParamUI;
	Shot shot[SHOT];

	// 各初期化を呼ぶ.
	player.Init();

	// std::function(OnDamageDelegate)は、メンバ関数への参照とクラスインスタンスの参照のセットである「関数オブジェクト」を扱う型
	// ほんとはこういうイメージで渡したい
	//OnDamageDelegate onDamage = (enemyParamUI.&OnDamage, &enemyParamUI);

	// しかしそう簡単にはいかない…
	// ↓のやり方で
	// &EnemyParamUI::OnDamage(メンバ関数へのポインタ)とenemyParamUI(クラスインスタンス)への参照から関数オブジェクトを生成し、
	// OnDamageDelegate（std::function）に代入できる
	//OnDamageDelegate onDamage = std::bind(std::mem_fn(&EnemyParamUI::OnDamage), std::ref(enemyParamUI), std::placeholders::_1);
	 
	// 細かい仕組みが多く、全部理解するのは大変！！
	// そこで、返り値が関数オブジェクトとなる匿名関数（ラムダ式）を作ることで、ほぼ同様の動作をさせる
	// ↑のやり方と違い、匿名関数の中で何か処理できてしまうところが難点ではあるが、見ればわかるので別に気にしない
	// これを機に匿名関数もバンバン覚えたいところ
	OnDamageDelegate onDamage = [&enemyParamUI](int damage) -> void { enemyParamUI.OnDamage(damage); };
	enemy.Init(onDamage);
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