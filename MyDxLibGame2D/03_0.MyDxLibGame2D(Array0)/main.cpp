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

	// 座標を準備
	VECTOR pos;

	// 位置を代入
	pos = VGet(PlayerDefaltPos.x,
		PlayerDefaltPos.y,
		PlayerDefaltPos.z);

	while (true)
	{
		// 画面に描かれているものを一回全部消す
		ClearDrawScreen();

		// 画面に絵を表示
		for (size_t i = 0; i < ArrayNum; i++)
		{
			DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), graph, FALSE);
		}

		// 矢印キーの→を押したら右に動かす.
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			pos = VAdd(pos, PlayerVelocity);
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
