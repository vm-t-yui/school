//-----------------------------------------------------------------------------
// @brief  物理ライブラリテスト
// 2024 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Screen.h"
#include "YuiLib.h"
#include "Player.h"
#include "Camera.h"

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

	// 初期化（+Physicsへの登録）
	Player* player = new Player;

	// ゲームループ.
	LONGLONG frameTime = 0;
	while (1)
	{
		frameTime = GetNowHiPerformanceCount();

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
			player->Update();
			camera->Update();
;
			SetUseLighting(TRUE);			// ライティング使う
			SetWriteZBufferFlag(TRUE);		// Ｚバッファへの書き込みを行う

			player->Draw();

			// 一番上に表示したいので、最後にデバッグ情報表示
#ifdef _DEBUG
			SetUseLighting(FALSE);			// ライティング使わない
			SetWriteZBufferFlag(FALSE);		// Ｚバッファへの書き込みをしない
			YuiLib::DebugDraw::Draw3D();
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

		// 雑に60fps固定
		while ((GetNowHiPerformanceCount() - frameTime) < 16667) {}
	}

	delete(camera);
	delete(player);

#ifdef _DEBUG
	YuiLib::DebugDraw::Finalize();
#endif

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}