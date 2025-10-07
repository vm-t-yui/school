// std::vectorを使うかどうかを0と1で使い分け。1にすると#if USE_VECTORがコンパイルされる
#define USE_VECTOR 1

#if USE_VECTOR
	#include <vector>
#endif

#include "DxLib.h"

// 配列の個数
constexpr int	ArrayNum		= 10;
const VECTOR	PlayerDefaltPos = VGet(0, 100, 0);
constexpr int	PlayerW			= 64;
const VECTOR	PlayerVelocity	= VGet(2.5f, 0, 0);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ＤＸライブラリ初期化
	if (DxLib_Init() == -1) return -1;

	// 画面をウインドウモードにする.
	ChangeWindowMode(TRUE);

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// とりあえずロード
	int graph = LoadGraph("data/texture/player.png");

	// 座標のコレクション（塊）を準備
#if !USE_VECTOR		// !をつけることで判定を反転させられる。USE_VECTOR = 0の時コンパイルされる
	VECTOR posArray[ArrayNum];	// 構造体の配列なので手動初期化しなければならない
#else
	// ArrayNum個のVECTOR型コンテナ（箱）を準備し、全部ゼロのベクトルで初期化する
	std::vector<VECTOR> posArray(ArrayNum, VGet(0, 0, 0));	// stdのコンテナは全部同じ値で初期化できる。べんり
#endif

	// 位置を計算して代入。iを使いたいときは通常のfor文
	for (int i = 0; i < ArrayNum; i++)
	{
		// 毎ループごとに1ずつ増えるiを使って座標を計算。だんだん右に。
		posArray[i] = VGet(PlayerDefaltPos.x + PlayerW * i,
			PlayerDefaltPos.y,
			PlayerDefaltPos.z);
	}

	while (true)
	{
		// 画面に描かれているものを一回全部消す
		ClearDrawScreen();

		// 画面に絵を表示
#if !USE_VECTOR
		for (int i = 0; i < ArrayNum; i++)
		{
			DrawGraph(static_cast<int>(posArray[i].x), static_cast<int>(posArray[i].y), graph, FALSE);
		}
#else
		// 範囲for文。xの中を一個ずつ見る
		// intの参照型のposが、xの中身を順番に参照する
		// 中身をいじらないときはconstつける
		for (const auto& pos : posArray)
		{
			DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), graph, FALSE);
		}
#endif

		// 矢印キーの→を押したら右に動かす.
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
#if !USE_VECTOR
			for (int i = 0; i < ArrayNum; i++)
			{
				posArray[i] = VAdd(posArray[i], PlayerVelocity);
			}
#else
			// 参照なのでいじれる。いじるときはconstはずす
			for (auto& pos : posArray)
			{
				pos = VAdd(pos, PlayerVelocity);
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
