//-----------------------------------------------------------------------------
// @brief  物理ライブラリテスト
// 2024 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Screen.h"
//#include "GameObject.h"
#include "YuiLib.h"
#include "Player.h"
#include "Enemy.h"
#include "SystemWall.h"
#include "Camera.h"
#include "StepGround.h"
#include "BackGround3D.h"
#include "BackGround2D.h"
#include "FrontGround2D.h"

#ifdef _DEBUG
// デバッグの状況
enum class DebugState
{
	Normal,	// 通常
	Pause,	// ポーズ中
};
DebugState debugState = DebugState::Normal;
bool prevPressedP = false;
bool prevPressedEnter = false;
#endif

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

#ifdef _DEBUG
	YuiLib::DebugDraw::Initialize();
#endif

	Camera* camera = new Camera;
	BackGround3D* backGround3D = new BackGround3D;
	BackGround2D* backGround2D = new BackGround2D;
	//FrontGround2D* frontGround2D = new FrontGround2D;

	// 物理
	YuiLib::Physics* physics = new YuiLib::Physics;

	// 初期化（+Physicsへの登録）
	Player* player = new Player;
	player->Initialize(physics);
	Enemy* enemy = new Enemy;
	enemy->Initialize(physics);
	SystemWall* systemWallRight = new SystemWall(VGet(-5.0f, 0.0f, 0.0f), VGet(-5.0f, 5.0f, 0.0f));
	SystemWall* systemWallLeft = new SystemWall(VGet(5.0f, 0.0f, 0.0f), VGet(5.0f, 5.0f, 0.0f));
	systemWallRight->Initialize(physics);
	systemWallLeft->Initialize(physics);
	StepGround* stepGround1 = new StepGround(VGet(-5.0f, 0.0f, 0.0f), VGet(5.0f, 0.0f, 0.0f));
	StepGround* stepGround2 = new StepGround(VGet(-5.0f, 2.5f, 0.0f), VGet(1.0f, 2.5f, 0.0f));
	stepGround1->Initialize(physics);
	stepGround2->Initialize(physics);

	// ゲームループ.
	while (1)
	{
#ifdef _DEBUG
		// キー入力取得
		char keyBuf[256];
		GetHitKeyStateAll(keyBuf);
		bool pressedP = false;
		bool pressedEnter = false;
		if (keyBuf[KEY_INPUT_P])
		{
			if (!prevPressedP)
			{
				pressedP = true;
			}
			prevPressedP = true;
		}
		else
		{
			prevPressedP = false;
		}
		if (keyBuf[KEY_INPUT_RETURN])
		{
			if (!prevPressedEnter)
			{
				pressedEnter = true;
			}
			prevPressedEnter = true;
		}
		else
		{
			prevPressedEnter = false;
		}

		// Pキーでポーズ、ポーズ中にPでコマ送り
		// ポーズ中にエンターで再開
		if (debugState == DebugState::Normal && pressedP)
		{
			debugState = DebugState::Pause;
		}
		if (debugState == DebugState::Pause && pressedEnter)
		{
			debugState = DebugState::Normal;
		}
		if (debugState != DebugState::Pause || pressedP)
		{
#endif
			// 画面を初期化(真っ黒にする)
			ClearDrawScreen();

			// デバッグ情報クリア
#ifdef _DEBUG
			YuiLib::DebugDraw::Clear();
#endif
			// 更新（物理移動をする前情報の決定。方向やパワー確定等）
			player->Update(physics);
			enemy->Update();
			systemWallRight->Update();
			systemWallLeft->Update();
			stepGround1->Update();
			stepGround2->Update();

			camera->Update();
			backGround2D->Update();
			backGround3D->Update();
			//frontGround2D->Update();
			
			physics->Update();	// 物理更新

			SetUseLighting(FALSE);			// ライティング使わない
			SetWriteZBufferFlag(FALSE);		// Ｚバッファへの書き込みをしない
			backGround2D->Draw();
			SetUseLighting(TRUE);			// ライティング使う
			SetWriteZBufferFlag(TRUE);		// Ｚバッファへの書き込みを行う
			backGround3D->Draw();

			SetUseLighting(FALSE);			// ライティング使わない
			player->Draw();
			enemy->Draw();
			SetUseLighting(TRUE);			// ライティング使う
			stepGround1->Draw();
			stepGround2->Draw();

			// 一番上に表示したいので、最後にデバッグ情報表示
#ifdef _DEBUG
			SetUseLighting(FALSE);			// ライティング使わない
			SetWriteZBufferFlag(FALSE);		// Ｚバッファへの書き込みをしない
			YuiLib::DebugDraw::Draw3D();
#endif

			//frontGround2D->Draw();

			// 裏画面の内容を表画面にコピーする（描画の確定）.
			ScreenFlip();

#ifdef _DEBUG
		}
#endif

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
	//delete(frontGround2D);
	player->Finalize(physics);	// 終了処理＋Physicsからの削除
	enemy->Finalize(physics);	// 終了処理＋Physicsからの削除
	systemWallRight->Finalize(physics);	// 終了処理＋Physicsからの削除
	systemWallLeft->Finalize(physics);	// 終了処理＋Physicsからの削除
	delete(player);
	delete(enemy);
	delete(stepGround1);
	delete(stepGround2);
	delete(systemWallRight);
	delete(systemWallLeft);
	delete(physics);

#ifdef _DEBUG
	YuiLib::DebugDraw::Finalize();
#endif

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}