// 2023 Takeru Yui All Rights Reserved.
#include "Shot.h"

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
/// <param name="enemy">敵</param>
/// <param name="enemy2">敵2</param>
/// <param name="enemy3">敵3</param>
void Shot::Update(Enemy& enemy, Enemy2& enemy2, Enemy3& enemy3)
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
	if (visibleFlag == 1 && enemy.GetLife() > 0)
	{
		// エネミーとの当たり判定
		if (((x > enemy.GetX() && x < enemy.GetX() + enemy.GetW()) ||
			(enemy.GetX() > x && enemy.GetX() < x + w)) &&
			((y > enemy.GetY() && y < enemy.GetY() + enemy.GetH()) ||
				(enemy.GetY() > y && enemy.GetY() < y + h)))
		{
			// 接触している場合は当たった弾の存在を消す
			visibleFlag = 0;

			enemy.OnDamage();
		}
	}
	if (visibleFlag == 1 && enemy2.GetLife() > 0)
	{
		// エネミーとの当たり判定
		if (((x > enemy2.GetX() && x < enemy2.GetX() + enemy2.GetW()) ||
			(enemy2.GetX() > x && enemy2.GetX() < x + w)) &&
			((y > enemy2.GetY() && y < enemy2.GetY() + enemy2.GetH()) ||
				(enemy2.GetY() > y && enemy2.GetY() < y + h)))
		{
			// 接触している場合は当たった弾の存在を消す
			visibleFlag = 0;

			enemy2.OnDamage();
		}
	}
	if (visibleFlag == 1 && enemy3.GetLife() > 0)
	{
		// エネミーとの当たり判定
		if (((x > enemy3.GetX() && x < enemy3.GetX() + enemy3.GetW()) ||
			(enemy3.GetX() > x && enemy3.GetX() < x + w)) &&
			((y > enemy3.GetY() && y < enemy3.GetY() + enemy3.GetH()) ||
				(enemy3.GetY() > y && enemy3.GetY() < y + h)))
		{
			// 接触している場合は当たった弾の存在を消す
			visibleFlag = 0;

			enemy3.OnDamage();
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void Shot::Draw() const
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