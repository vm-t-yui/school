// 2023 Takeru Yui All Rights Reserved.
#include "Shot.h"
#include "EnemyBase.h"

/// <summary>
/// 初期化
/// </summary>
void Shot::Init()
{
	// ショットのグラフィックをメモリにロード.
	graph = LoadGraph("data/texture/SuperShot.png");

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	visibleFlag = false;

	// 弾のグラフィックのサイズをえる
	GetGraphSize(graph, &w, &h);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="enemies">敵リスト</param>
/// <param name="enemyNum">敵リストの数</param>
void Shot::Update(EnemyBase* enemies[], int enemyNum)
{
	// 自機の弾iの移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
	if (visibleFlag == true)
	{
		// 弾iを移動させる
		y -= SHOT_SPEED;

		// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
		if (y < 0 - h)
		{
			visibleFlag = false;
		}
	}

	// 弾のあたり判定.
	// 弾iが存在している場合のみ次の処理に映る
	if (visibleFlag == 1)
	{
		for (int i = 0; i < enemyNum; i++)
		{
			// 死んでる敵は当たり判定しない
			if (enemies[i]->GetLife() > 0)
			{
				// エネミーとの当たり判定
				if (((x > enemies[i]->GetX() && x < enemies[i]->GetX() + enemies[i]->GetW()) ||
					(enemies[i]->GetX() > x && enemies[i]->GetX() < x + w)) &&
					((y > enemies[i]->GetY() && y < enemies[i]->GetY() + enemies[i]->GetH()) ||
						(enemies[i]->GetY() > y && enemies[i]->GetY() < y + h)))
				{
					// 接触している場合は当たった弾の存在を消す
					visibleFlag = 0;

					enemies[i]->OnDamage();
				}
			}
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void Shot::Draw()
{
	if (visibleFlag == true)
	{
		// 画面に弾iを描画する
		DrawGraph(x, y, graph, TRUE);
	}
}

/// <summary>
/// ショット撃った
/// </summary>
/// <param name="playerX">プレイヤーのx座標</param>
/// <param name="playerY">プレイヤーのy座標</param>
void Shot::OnShot(int playerX, int playerY)
{
	// 弾iの位置をセット、位置はプレイヤーの中心にする
	x = (w) / 2 + playerX;
	y = (h) / 2 + playerY;

	// 弾iは現時点を持って存在するので、存在状態を保持する変数にtrueを代入する
	visibleFlag = true;
}