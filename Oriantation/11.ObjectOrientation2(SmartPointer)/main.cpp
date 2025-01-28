//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include <memory>
#include "DxLib.h"
#include "Common.h"
#include "Shot.h"
#include "Player.h"
#include "Enemy.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------

//----------------------------//
// グローバル変数.
//----------------------------//
//Player* player = nullptr;
std::shared_ptr<Player> player = nullptr;
std::shared_ptr<Enemy>  enemy = nullptr;

// 元々のSHOT個数分に加えてSHOT個追加でnewされてしまうので、事前確保はしない
ShotContainer shot;

//----------------------------//
// WinMain関数.
//----------------------------//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(SCREEN_W, SCREEN_H, 16);		// 解像度をSCREEN_W*SCREEN_H、colorを16bitに設定.
	ChangeWindowMode(TRUE);						// ウインドウモードに.
	
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// 背景のスクロールをするためにテクスチャモードをラッピングに修正.
	SetTextureAddressMode(DX_TEXADDRESS_WRAP);

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// 各初期化を呼ぶ.
	//player = new Player();
	player = std::make_shared<Player>();
	player->Init();
	//enemy = new Enemy();
	enemy = std::make_shared<Enemy>();
	enemy->Init();

	// 範囲for分
	for (int i = 0; i < SHOT; i++)
	{
		shot.emplace_back(std::make_shared<Shot>());
	}
	for (auto& item : shot)
	{
		item->Init();
	}

	// ゲームループ.
	while (1)
	{
		// 画面を初期化(真っ黒にする)
		ClearDrawScreen();

		// 各アップデート関数を呼ぶ.
		player->Update(shot);
		enemy->Update();

		// Xキーを押したらショットの中身が増える
		if (CheckHitKey(KEY_INPUT_X) == 1)
		{
			// このままだと無駄にローカル変数のコピーが残る
			//Shot newShot;
			//newShot.Init();
			// ポインタ型になったことで、ローカル変数がコピーされても
			// 実体は一つなので問題がない
			std::shared_ptr<Shot> newShot = std::make_shared<Shot>();
			newShot->Init();
			shot.emplace_back(newShot);
		}
		// Zキーを押したらショットの中身が消える
		if (CheckHitKey(KEY_INPUT_Z) == 1 && shot.size() > 0)
		{
			// 個別に削除するなら、「削除するものリスト」みたいなものを準備して選んで削除する必要がある
			// 例：先頭を削除
#if USE_LIST
			// list
			std::shared_ptr<Shot> deleteItem = nullptr;
			for (auto& item : shot)
			{
				if (true)	// なんらかの条件に合致したら削除
				{
					// 削除予定のものを覚えておく
					// for分の途中で消すと危険
					deleteItem = item;
				}
				break;
			}
			if (deleteItem != nullptr)
			{
				shot.remove(deleteItem);
				//delete(deleteItem);
			}
#else
			// vector
			int deleteItemIndex = 0;
			//delete(shot[deleteItemIndex]);
			shot.erase(shot.begin() + deleteItemIndex);
#endif
		}
		printfDx("弾の総数%d\n", shot.size() );
		for (auto& item : shot)
		{
			// Enemyの大元がポインタ型になったからといって、ポインタ型を渡さなくてよい
			// ポインタがさす先の実体を渡してあげれば、nullが来ることはない
			// さらに、書き換える箇所も減る
			item->Update(*enemy);
		}

		// 各描画関数を呼ぶ.
		for (auto& item : shot)
		{
			item->Draw();
		}
		player->Draw();
		enemy->Draw();

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
	
	//// 範囲回して全部削除する
	//for (auto& item : shot)
	//{
	//	// shotのポインタの指し示す先が削除されるので問題なし
	//	delete(item);
	//}
	// ただし、削除したらコンテナの中身も削除する必要がある
	shot.clear();

	//delete(player);
	player = nullptr;
	//delete(enemy);
	enemy = nullptr;

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}