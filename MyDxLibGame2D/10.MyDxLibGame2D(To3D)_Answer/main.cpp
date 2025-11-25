//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include <vector>
#include "GlobalConstants.h"
#include "Input.h"
#include "Enemy.h"
#include "Shot.h"
#include "Player.h"
#include "Camera.h"

/// <summary>
/// メイン関数
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(Graphics::ScreenW, Graphics::ScreenH, Graphics::ColorBit);	// 解像度、colorbit設定
	ChangeWindowMode(TRUE);							// ウインドウモードに

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	///////////////////////////////////////////////////////////////////
	// グラフィック設定
	SetDrawScreen(DX_SCREEN_BACK);	// 裏画面を描画対象にする
	SetUseZBufferFlag(TRUE);		// Ｚバッファを使用する
	SetWriteZBufferFlag(TRUE);		// Ｚバッファへの書き込みを行う
	SetUseBackCulling(TRUE);		// バックカリングを行う
	///////////////////////////////////////////////////////////////////
	
	// 各クラスのインスタンス準備
	Input	input;
	Player	player;
	Enemy	enemy;
	Camera	camera;
	std::vector<Shot>	shotArray(Shot::Num);

	// 各クラスの初期化
	//Input::Initialize(input);	// static関数ならこれ
	input.Initialize();	// クラスごとに準備される関数なので、.でアクセス
	player.Initialize();
	enemy.Initialize();

	//for (int i = 0; i < Shot::Num; i++)
	//{
	//	shot[i].Initialize();
	//}
	for (auto& item : shotArray)
	{
		item.Initialize();
	}

	// ゲームループ.
	while (1)
	{
		auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間

		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();
		
		// 各クラスのUpdate
		input.Update();		// ボタン情報を更新
		player.Update(input, shotArray);
		enemy.Update();
		for (auto& item : shotArray)
		{
			item.Update(enemy);
		}

		// 各クラスのDraw
		player.Draw();
		enemy.Draw();
		for (auto& item : shotArray)
		{
			item.Draw();
		}

#if _DEBUG
		camera.DrawDebug();	// デバッグ描画
#endif // _DEBUG

		
		// 裏画面の内容を表画面にコピーする（描画の確定）
		ScreenFlip();

		// 雑なfps固定処理
		// 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
		auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
		while (afterTime - prevTime < Graphics::OneFrameNanoSec)
		{
			afterTime = GetNowHiPerformanceCount();
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