
#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ＤＸライブラリ初期化
	if (DxLib_Init() == -1) return -1;

	// 画面をウインドウモードにする.
	ChangeWindowMode(TRUE);

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// x座標.
	int x = 0;

	while (true)
	{
		// 画面に描かれているものを一回全部消す
		ClearDrawScreen();

		// 画面に絵を表示
		LoadGraphScreen(x, 100, "data/texture/player.png", FALSE);

		// 矢印キーの→を押したら右に動かす.
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			x++;
		}

		// エスケープキーを押したら抜ける.
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	// ＤＸライブラリ使用の終了
	DxLib_End();

	// ソフトの終了
	return 0;
}
