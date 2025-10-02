// ベクトルを使うかどうかを0と1で使い分け。1にすると#if USE_VECTORがコンパイルされる
#define USE_VECTOR 0

#if USE_VECTOR
	#include <vector>
#endif

#include "DxLib.h"

// 配列の個数
constexpr int ArrayNum = 10;
constexpr int PlayerY = 100;
constexpr int PlayerW = 64;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ＤＸライブラリ初期化
	if (DxLib_Init() == -1) return -1;

	// 画面をウインドウモードにする.
	ChangeWindowMode(TRUE);

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// x座標のコレクション（塊）を準備
#if !USE_VECTOR		// !をつけることで判定を反転させられる。USE_VECTOR = 0の時コンパイルされる
	int x[ArrayNum];
#else
	std::vector<int> x(ArrayNum);
#endif

	// 位置を計算して代入。iを使いたいときは通常のfor文
	for (int i = 0; i < ArrayNum; i++)
	{
		x[i] = PlayerW * i;	// 毎ループごとに1ずつ増えるiを使って座標を計算。だんだん右に。
	}

	while (true)
	{
		// 画面に描かれているものを一回全部消す
		ClearDrawScreen();

		// 画面に絵を表示
#if !USE_VECTOR
		for (int i = 0; i < ArrayNum; i++)
		{
			LoadGraphScreen(x[i], PlayerY, "data/texture/player.png", FALSE);
		}
#else
		// いじらないときはconstつける
		for (const auto& item : x)
		{
			LoadGraphScreen(item, PlayerY, "data/texture/player.png", FALSE);
		}
#endif

		// 矢印キーの→を押したら右に動かす.
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
#if !USE_VECTOR
			for (int i = 0; i < ArrayNum; i++)
			{
				x[i]++;
			}
#else
			// 参照なのでいじれる。いじるときはconstはずす
			for (auto& item : x)
			{
				item++;
			}
#endif
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
