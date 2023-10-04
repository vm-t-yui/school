//-----------------------------------------------------------------------------
// @brief  ショット処理.
//-----------------------------------------------------------------------------
#include "Shot.h"
#include "Enemy.h"
#include "EnemyParam.h"

//----------------------------//
// ショット関数群.
//----------------------------//
// 初期化.
void Shot::Init()
{
	// ショットのグラフィックをメモリにロード.
	Graph = LoadGraph("data/texture/SuperShot.png");

	// 弾が画面上に存在しているか保持する変数に『存在していない』を意味するfalseを代入しておく
	VisibleFlag = false;

	// 弾のグラフィックのサイズをえる
	GetGraphSize(Graph, &W, &H);
}

// アップデート.
void Shot::Update(Enemy& enemy, const EnemyParam& param)
{
	// 自機の弾iの移動ルーチン( 存在状態を保持している変数の内容がtrue(存在する)の場合のみ行う )
	if (VisibleFlag == true)
	{
		// 弾iを移動させる
		Y -= SHOT_SPEED;

		// 画面外に出てしまった場合は存在状態を保持している変数にfalse(存在しない)を代入する
		if (Y < 0 - H)
		{
			VisibleFlag = false;
		}
	}

	// 弾のあたり判定.
	// 弾iが存在している場合のみ次の処理に映る
	if (VisibleFlag == 1 && param.Life > 0)
	{
		// エネミーとの当たり判定
		if (((X > enemy.X && X < enemy.X + enemy.W) ||
			(enemy.X > X && enemy.X < X + W)) &&
			((Y > enemy.Y && Y < enemy.Y + enemy.H) ||
				(enemy.Y > Y && enemy.Y < Y + H)))
		{
			// 接触している場合は当たった弾の存在を消す
			VisibleFlag = 0;

			enemy.OnDamage(1);
		}
	}
}
// 描画.
void Shot::Draw()
{
	if (VisibleFlag == true)
	{
		// 画面に弾iを描画する
		DrawGraph(X, Y, Graph, TRUE);
	}
}