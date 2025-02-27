// 2025 Takeru Yui All Rights Reserved.
#include <memory>
#include <vector>
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include "ObstacleBase.h"
#include "ObstacleKinematic.h"
#include "ObstacleMovable.h"

// 障害物の数
const int ObstacleNum = 3;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	// 画面モードの設定
	SetGraphMode(640, 480, 16);		// 解像度を640*480、colorを16bitに設定.
	ChangeWindowMode(TRUE);			// ウインドウモードに.

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		return -1;	// エラーが起きたら直ちに終了
	}

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBufferFlag(TRUE);		// Ｚバッファを使用する
	SetUseBackCulling(TRUE);		// バックカリングを行う

	// カメラを生成
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();

	// プレイヤーを生成
	std::shared_ptr<Player> player = std::make_shared<Player>();

	// 障害物を生成
	std::vector< std::shared_ptr<ObstacleBase> > obstacles;
	obstacles.emplace_back( std::make_shared<ObstacleKinematic>() );
	obstacles.emplace_back( std::make_shared<ObstacleMovable>()) ;
	obstacles.emplace_back( std::make_shared<ObstacleKinematic>() );
	//std::shared_ptr<ObstacleBase>	obstacleKinematic1	= std::make_shared<ObstacleKinematic>();
	//std::shared_ptr<ObstacleBase>	obstacleMovable		= std::make_shared<ObstacleMovable>();
	//std::shared_ptr<ObstacleBase>	obstacleKinematic2	= std::make_shared<ObstacleKinematic>();
	//std::shared_ptr<ObstacleKinematic>	obstacleKinematic1	= std::make_shared<ObstacleKinematic>();
	//std::shared_ptr<ObstacleMovable>		bstacleMovable		= std::make_shared<ObstacleMovable>();
	//std::shared_ptr<ObstacleKinematic>	obstacleKinematic2	= std::make_shared<ObstacleKinematic>();

	// 障害物の位置を初期化
	float band = 10.0f;
	for (int i = 0; i < obstacles.size(); i++)
	{
		obstacles[i]->SetPos(VGet(-band + (band * i), 0, -1.0f));
	}
	//obstacles[0]->SetPos(VGet(-band + (band * 0), 0, -1.0f));
	//obstacles[1]->SetPos(VGet(-band + (band * 1), 0, -1.0f));
	//obstacles[2]->SetPos(VGet(-band + (band * 2), 0, -1.0f));
	//obstacleKinematic1->SetPos(VGet(-band + (band * 0), 0, -1.0f));
	//obstacleMovable->SetPos(VGet(-band + (band * 1), 0, -1.0f));
	//obstacleKinematic2->SetPos(VGet(-band + (band * 2), 0, -1.0f));

	// エスケープキーが押されるかウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間

		// プレイヤー制御
		player->Update();

		// カメラ制御
		camera->Update(*player);

		// 障害物制御
		for (const auto& item : obstacles)
		{
			item->Update();
		}
        //obstacleKinematic1->Update();
        //obstacleMovable->Update();
        //obstacleKinematic2->Update();

		// 画面を初期化する
		ClearDrawScreen();

		// プレイヤー描画
		player->Draw();

		// 障害物描画.
		for (const auto& item : obstacles)
		{
			item->Draw();
		}
        //obstacleKinematic1->Draw();
        //obstacleMovable->Draw();
        //obstacleKinematic2->Draw();

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		// 雑なfps固定処理
		// 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
		auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
		while (afterTime - prevTime < 16667)
		{
			afterTime = GetNowHiPerformanceCount();
		}
	}

	// メモリ解放
	player				= nullptr;
	camera				= nullptr;
	obstacles.clear();
	//obstacleKinematic1	= nullptr;
	//obstacleMovable		= nullptr;
	//obstacleKinematic2	= nullptr;

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}