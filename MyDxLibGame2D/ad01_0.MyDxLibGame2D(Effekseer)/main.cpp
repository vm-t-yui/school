#include "DxLib.h"
#include "EffekseerForDXLib.h"

namespace
{
	const static int WindowW = 640;
	const static int WindowH = 480;
}

// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(WindowW, WindowH, 16);		// 解像度を640*480、colorを16bitに設定.
	ChangeWindowMode(TRUE);			// ウインドウモードに.

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	//------------------------------//
	// エフェクトの初期化
	//------------------------------//
	{
		// DirectX9を使用するようにする。(DirectX9も可だが、機能が制限される)
		// Effekseerを使用するには必ず設定する。
		SetUseDirect3DVersion(DX_DIRECT3D_11);

		// Effekseerを初期化する。
		// 引数には画面に表示する最大パーティクル数を設定する。
		if (Effkseer_Init(8000) == -1)
		{
			DxLib_End();
			return -1;
		}

		// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
		// Effekseerを使用する場合は必ず設定する。
		SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

		// DXライブラリのデバイスロストした時のコールバックを設定する。
		// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
		// ただし、DirectX11を使用する場合は実行する必要はない。
		Effekseer_SetGraphicsDeviceLostCallbackFunctions();

		// Effekseerに2D描画の設定をする。
		Effekseer_Set2DSetting(WindowW, WindowH);

		// Zバッファを有効にする。
		// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
		SetUseZBuffer3D(TRUE);

		// Zバッファへの書き込みを有効にする。
		// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
		SetWriteZBuffer3D(TRUE);
	}

	// プレイヤーのグラフィックをメモリにロード＆表示座標を初期化
	int playerX, playerY, playerGraph;
	playerGraph = LoadGraph("data/texture/player.png");
	playerX = 288; 
	playerY = 400;

	// ショットのグラフィックをメモリにロード.
	int shotGraph;
	shotGraph = LoadGraph("data/texture/shot.png");

	// エフェクトリソースを読み込む。
	int effectResourceHandle = LoadEffekseerEffect("data/laser.efk");

	// 再生中のエフェクト
	int playingEffectHandle = -1;

	// 位置を初期化.
	int shotX, shotY;

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	bool shotFlag = false;

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		//------------------------------//
		// プレイヤーの操作ルーチン
		//------------------------------//
		{
			// 矢印キーを押していたらプレイヤーを移動させる
			if (CheckHitKey(KEY_INPUT_UP) == 1)
			{
				playerY -= 3;
			}
			if (CheckHitKey(KEY_INPUT_DOWN) == 1)
			{
				playerY += 3;
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				playerX -= 3;
			}
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
			{
				playerX += 3;
			}

			// 弾の発射処理
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				// 弾iが画面上にでていない場合はその弾を画面に出す
				if (shotFlag == false)
				{
					int Bw, Bh, Sw, Sh;

					// プレイヤーと弾の画像のサイズを得る
					GetGraphSize(playerGraph, &Bw, &Bh);
					GetGraphSize(shotGraph, &Sw, &Sh);

					// 弾iの位置をセット、位置はプレイヤーの中心にする
					shotX = (Bw - Sw) / 2 + playerX;
					shotY = (Bh - Sh) / 2 + playerY;

					// 弾iは現時点を持って存在するので、存在状態を保持する変数に１を代入する
					shotFlag = true;

					// エフェクトを再生する。
					playingEffectHandle = PlayEffekseer2DEffect(effectResourceHandle);

					// エフェクトの拡大率を設定する。
					// Effekseerで作成したエフェクトは2D表示の場合、小さすぎることが殆どなので必ず拡大する。
					SetScalePlayingEffekseer2DEffect(playingEffectHandle, 25.0f, 25.0f, 25.0f);
				}
			}

			// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
			if (playerX < 0)
			{
				playerX = 0;
			}
			if (playerX > 640 - 64)
			{
				playerX = 640 - 64;
			}
			if (playerY < 0)
			{
				playerY = 0;
			}
			if (playerY > 480 - 64)
			{
				playerY = 480 - 64;
			}

			// プレイヤーを描画
			DrawGraph(playerX, playerY, playerGraph, TRUE);
		}

		//------------------------------//
		// 弾の移動ルーチン
		//------------------------------//
		// 自機の弾の移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
		if (shotFlag == true)
		{
			// 弾iを１６ドット上に移動させる
			shotY -= 16;

			// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
			if (shotY < -80)
			{
				shotFlag = false;
			}

			// 画面に弾を描画する
			DrawGraph(shotX, shotY, shotGraph, TRUE);
		}

		//------------------------------//
		// エフェクトルーチン
		//------------------------------//
		if(playingEffectHandle >= 0) // 再生中エフェクトのハンドルがあれば.
		{
			// 再生中のエフェクトをたまと同じ位置に移動。
			SetPosPlayingEffekseer2DEffect(playingEffectHandle, (float)shotX, (float)shotY, 0);

			// Effekseerにより再生中のエフェクトを更新する。
			UpdateEffekseer2D();

			// Effekseerにより再生中のエフェクトを描画する。
			DrawEffekseer2D();
		}

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

	// エフェクトリソースを削除する。(Effekseer終了時に破棄されるので削除しなくてもいい)
	DeleteEffekseerEffect(effectResourceHandle);
	
	Effkseer_End();				// Effekseerを終了する。
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}