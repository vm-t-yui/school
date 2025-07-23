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
#include "Camera.h"
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

	//Camera* camera = new Camera;
	//BackGround3D* backGround3D = new BackGround3D;
	//BackGround2D* backGround2D = new BackGround2D;
	//FrontGround2D* frontGround2D = new FrontGround2D;

	// 物理
	YuiLib::Physics* physics = new YuiLib::Physics;

	// 初期化（+Physicsへの登録）
	Player* player = new Player;
	player->Initialize(physics);
	Enemy* enemy = new Enemy;
	enemy->Initialize(physics);

	// ゲームループ.
	while (1)
	{
		auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間

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
			player->Update();
			enemy->Update();

			//camera->Update();
			//backGround2D->Update();
			//backGround3D->Update();
			//frontGround2D->Update();
			
			physics->Update();	// 物理更新

			//SetWriteZBufferFlag(FALSE);		// Ｚバッファへの書き込みをしない
			//backGround2D->Draw();
			//SetWriteZBufferFlag(TRUE);		// Ｚバッファへの書き込みを行う
			//backGround3D->Draw();
			//SetWriteZBufferFlag(FALSE);		// Ｚバッファへの書き込みをしない
			player->Draw();
			enemy->Draw();

			//frontGround2D->Draw();

			// 一番上に表示したいので、最後にデバッグ情報表示
#ifdef _DEBUG
			YuiLib::DebugDraw::Draw();
#endif

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

		// 雑なfps固定処理
		// 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
		auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
		while (afterTime - prevTime < 16667)
		{
			afterTime = GetNowHiPerformanceCount();
		}
	}

	//delete(camera);
	//delete(backGround3D);
	//delete(backGround2D);
	//delete(frontGround2D);
	player->Finalize(physics);	// 終了処理＋Physicsからの削除
	delete(player);
	delete(enemy);
	delete(physics);

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}