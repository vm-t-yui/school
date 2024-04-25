//-----------------------------------------------------------------------------
// @brief  2Dと3Dの交互表示
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Screen.h"
#include "GameObject.h"
#include "Player.h"
#include "Camera.h"
#include "BackGround3D.h"
#include "BackGround2D.h"
#include "FrontGround2D.h"

// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16);		// 解像度を640*480、colorを16bitに設定.
	ChangeWindowMode(TRUE);			// ウインドウモードに.
	
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBufferFlag(TRUE);		// Ｚバッファを使用する
	SetUseBackCulling(TRUE);		// バックカリングを行う

	//Camera* camera = new Camera;
	//BackGround3D* backGround3D = new BackGround3D;
	//BackGround2D* backGround2D = new BackGround2D;
	//FrontGround2D* frontGround2D = new FrontGround2D;

	// プレイヤー初期化
	Player* player = new Player;
	player->Init();

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		player->Update();

		//camera->Update();
		//backGround2D->Update();
		//backGround3D->Update();
		//frontGround2D->Update();

		//SetWriteZBufferFlag(FALSE);		// Ｚバッファへの書き込みをしない
		//backGround2D->Draw();
		//SetWriteZBufferFlag(TRUE);		// Ｚバッファへの書き込みを行う
		//backGround3D->Draw();
		//SetWriteZBufferFlag(FALSE);		// Ｚバッファへの書き込みをしない
		
		player->Draw();

		//frontGround2D->Draw();

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

	//delete(camera);
	//delete(backGround3D);
	//delete(backGround2D);
	//delete(frontGround2D);
	delete(player);

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}