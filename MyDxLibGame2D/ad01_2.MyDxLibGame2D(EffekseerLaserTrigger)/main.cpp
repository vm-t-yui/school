#include "DxLib.h"
#include "EffekseerForDXLib.h"

namespace
{
	const static int WindowW = 640;
	const static int WindowH = 480;
}


enum class EffectState
{
	Stoped,		// 完全停止済
	Play,		// プレイ中
	SpawnStop	// 生成停止
};
int			playingEffectHandle = -1;	// 再生中のエフェクト
EffectState	effectState			= EffectState::Stoped;
VECTOR		effectPos			= VGet(WindowW * 0.5f, WindowH * 0.5f, 0);

// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(WindowW, WindowH, 32);		// 解像度を640*480
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
		// DirectX11を使用するようにする。(DirectX9も可だが機能が制限される)
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

	// エフェクトリソースを読み込む
	// ループで無限再生されるエフェクトは、パーティクル自体の生成数が無限だったり、
	// パーティクルの寿命が無限だったりする
	// ※注意！※　この炎エフェクトは寿命が一定のパーティクルが無限生成されることでループを表現している
	// 寿命が無限だと粒子がずっと死なないのでメモリが無限に食われる
	// なお、エフェクトファイル自体に「ループ」という設定は存在しないので、エフェクトファイル左下の時間をゲーム時間より長くする必要がある
	// https://effekseer.github.io/Helps/17x/Tool/ja/ToolTips/01.html
	int effectResourceHandle = LoadEffekseerEffect("data/loopFire.efk");

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		// 再生、生成停止、完全停止
		switch (effectState)
		{
		case EffectState::Stoped:
			DrawString(0, 0, "完全停止（１で再生）", GetColor(255, 255, 255));

			// 1で再生
			if (CheckHitKey(KEY_INPUT_1) && playingEffectHandle < 0)
			{
				// エフェクトを再生する。
				playingEffectHandle = PlayEffekseer2DEffect(effectResourceHandle);

				// エフェクトの拡大率を設定する。
				// Effekseerで作成したエフェクトは2D表示の場合、小さすぎることが殆どなので必ず「エクスポート時に」拡大する。
				// そのうえで多少のサイズ調整はこちらで行う（あまり極端にいじると見た目が破綻する
				//SetScalePlayingEffekseer2DEffect(playingEffectHandle, 1.2f, 1.2f, 1.2f);
				effectState = EffectState::Play;
			}
			break;

		case EffectState::Play:
			DrawString(0, 0, "再生中（２で生成停止、３で停止）", GetColor(255, 255, 255));

			// 2で生成停止
			if (CheckHitKey(KEY_INPUT_2) && playingEffectHandle >= 0)
			{
				// エフェクト側で生成停止トリガーを仕込み、それを呼ぶ
				// Effekseerのマネージャーを取得し、そこに対してトリガーを呼ぶ
				// トリガー0をEffekeer側で「生成停止」に割り当てているので、生成停止される
				// https://effekseer.github.io/Helps/17x/Tool/ja/ToolTutorial/15.html
				// リンク先の通り、チャージと発射などもエフェクト側で指定されていれば実現できる
				auto manager = GetEffekseer2DManager();	// 3Dなら3Dマネージャ
				manager.Get()->SendTrigger(playingEffectHandle, 0);

				effectState = EffectState::SpawnStop;
			}
			// 3で完全停止
			else if (CheckHitKey(KEY_INPUT_3) && playingEffectHandle >= 0)
			{
				StopEffekseer2DEffect(playingEffectHandle);
				playingEffectHandle = -1;
				effectState = EffectState::Stoped;
			}
			break;

		case EffectState::SpawnStop:
			DrawString(0, 0, "生成停止中（３で停止）", GetColor(255, 255, 255));

			// 3で完全停止
			if (CheckHitKey(KEY_INPUT_3) && playingEffectHandle >= 0)
			{
				StopEffekseer2DEffect(playingEffectHandle);
				playingEffectHandle = -1;
				effectState = EffectState::Stoped;
			}
			break;
		default:
			break;
		}

		//------------------------------//
		// エフェクトルーチン
		//------------------------------//
		if(playingEffectHandle >= 0) // 再生中エフェクトのハンドルがあれば.
		{
			// 再生中のエフェクトを移動
			SetPosPlayingEffekseer2DEffect(playingEffectHandle, effectPos.x, effectPos.y, effectPos.z);

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