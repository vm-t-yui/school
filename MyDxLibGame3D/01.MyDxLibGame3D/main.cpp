//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Player.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;	// エラーが起きたら直ちに終了
	}

	// 画面モードのセット
	SetGraphMode(640, 480, 16);
	ChangeWindowMode(TRUE);

	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 1000.0f);

	//(0,10,-20)の視点から(0,10,0)のターゲットを見る角度にカメラを設置
	SetCameraPositionAndTarget_UpVecY(VGet(0, 10, -20), VGet(0.0f, 10.0f, 0.0f));

	// ３Ｄモデルの読み込み
	int		modelHandle;
	modelHandle = MV1LoadModel("data/model/player/hackadoll.pmx");

	VECTOR	pos;
	pos = VGet(0, 0, 0);

	// エスケープキーが押されるかウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// プレイヤー制御.
		int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// 上を押していたら上に進む
		if (key & PAD_INPUT_UP)
		{
			pos.z += 1;
		}

		// 下を押していたら下に進む
		if (key & PAD_INPUT_DOWN)
		{
			pos.z -= 1;
		}

		// 右を押していたら右に進む
		if (key & PAD_INPUT_RIGHT)
		{
			pos.x += 1;
		}

		// 左を押していたら左に進む
		if (key & PAD_INPUT_LEFT)
		{
			pos.x -= 1;
		}

		// ３Dモデルのポジション設定
		MV1SetPosition(modelHandle, pos);

		// 画面を初期化する
		ClearDrawScreen();

		// プレイヤー描画.
		MV1DrawModel(modelHandle);

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	// プレイヤーを削除.
	MV1DeleteModel(modelHandle);

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}