//-----------------------------------------------------------------------------
// @brief  2Dと3Dの交互表示
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Common.h"
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
#include "BackGround3D.h"
#include "BackGround2D.h"
#include "FrontGround2D.h"

// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	/*///////////////////////////////////
	素材は
	school\ChallengeCampas\data\texture
	にあります

	1.BackGround2Dクラスを作成し、BackGround3Dより後ろに背景を表示する
	（FancyBG_back.png）
	2.FrontGround2Dクラスを作成し、すべての表示より手前に背景を表示する
	（FancyBG_front.png）
	3.WorldSprite.h/cppをschooll\MyDxLibGame2.5D\01_0.MyDxLibGame2.5Dからコピー
	4.BackGround3Dクラスの中で、WorldSpriteで何かしらの絵を表示してみる
	5.WorldSpriteのポジションや描画順をいじって、3D表示の描画ルールを理解する
	///////////////////////////////////*/
	
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

	// ZBufferとは
	//SetUseZBufferFlag(TRUE);		// Ｚバッファを使用する
	//SetUseBackCulling(TRUE);		// バックカリングを行う
	//SetWriteZBufferFlag(TRUE);		// Ｚバッファへの書き込みを行う

	Camera* camera = new Camera;
	BackGround3D* backGround3D = new BackGround3D;
	BackGround2D* backGround2D = new BackGround2D;
	FrontGround2D* frontGround2D = new FrontGround2D;

	// プレイヤー初期化
	Player player;
	InitPlayer(player);

	// エネミー初期化.
	Enemy enemy;
	InitEnemy(enemy);

	//////////////////////
	// ベクトル計算のテスト.
	Vector v1, v2, v3;
	v1.x = 0;
	v1.y = 1;
	v2.x = 2;
	v2.y = 3;
	v3 = AddVector(v1, v2);
	printfDx("x:%f y:%f\n", v3.x, v3.y);
	v3 = SubVector(v1, v2);
	printfDx("x:%f y:%f\n", v3.x, v3.y);
	printfDx("len:%f\n", GetVectorLength(v3));
	v3 = NormalizeVector(v3);
	printfDx("x:%f y:%f\n", v3.x, v3.y);
	//////////////////////

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		UpdatePlayer(player);
		UpdateEnemy(enemy, player);

		camera->Update();
		backGround2D->Update();
		backGround3D->Update();
		frontGround2D->Update();

		//SetWriteZBufferFlag(FALSE);		// Ｚバッファへの書き込みをしない
		backGround2D->Draw();
		//SetWriteZBufferFlag(TRUE);		// Ｚバッファへの書き込みを行う
		backGround3D->Draw();
		//SetWriteZBufferFlag(FALSE);		// Ｚバッファへの書き込みをしない
		DrawPlayer(player);
		DrawEnemy(enemy);
		frontGround2D->Draw();

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

	delete(camera);
	delete(backGround3D);
	delete(backGround2D);
	delete(frontGround2D);

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}